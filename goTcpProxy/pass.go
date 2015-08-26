package main

import (
	"flag"
	"fmt"
	"log"
	"net"
	"time"
)

var (
	src          = flag.String("src", "127.0.0.1:8999", "The proxy server's host.")
	dest         = flag.String("dest", "172.16.1.250:3306", "where proxy server forward requests to.")
	maxConn      = flag.Int("max_avail_conn", 25, "The max active connection at any given time.")
	maxWaitQueue = flag.Int("max_wait_conn", 10000, "The max connections that can be waiting to be served.")
	expire       = flag.Int("ttl", 20, "time out of read and write")
)

func main() {

	flag.Parse()
	fmt.Printf("Proxying %s->%s.\n", *src, *dest)

	server, err := net.Listen("tcp", *src)
	if err != nil {
		log.Fatal(err)
	}

	waitQueue := make(chan net.Conn, *maxWaitQueue)
	availPools := make(chan bool, *maxConn)
	for i := 0; i < *maxConn; i++ {
		availPools <- true
	}

	go loop(waitQueue, availPools)

	for {
		connection, err := server.Accept()
		if err != nil {
			log.Print(err)
		} else {
			log.Printf("Received connection from %s.\n", connection.RemoteAddr())
			waitQueue <- connection
		}
	}
}

func loop(waitQueue chan net.Conn, availPools chan bool) {
	for connection := range waitQueue {
		<-availPools
		go func(connection net.Conn) {
			handleConnection(connection)
			availPools <- true
			log.Printf("Closed connection from %s.\n", connection.RemoteAddr())
		}(connection)
	}
}

func handleConnection(connection net.Conn) {
	defer connection.Close()

	remote, err := net.Dial("tcp", *dest)
	defer remote.Close()

	if err != nil {
		log.Print(err)
		return
	}

	//等待双向连接完成
	complete := make(chan bool, 2)
	one_side := make(chan bool, 1)
	other_side := make(chan bool, 1)
	go pass(connection, remote, complete, one_side, other_side)
	go pass(remote, connection, complete, other_side, one_side)
	<-complete
	<-complete
}

func pass(from net.Conn, to net.Conn, complete chan bool, done chan bool, otherDone chan bool) {
	var err error = nil
	var bytes []byte = make([]byte, 256)
	var read int = 0

	for {
		select {

		case <-otherDone:
			complete <- true
			return

		default:

			from.SetReadDeadline(time.Now().Add(time.Duration(*expire) * time.Second))
			read, err = from.Read(bytes)
			if err != nil {
				complete <- true
				done <- true
				return
			}

			to.SetWriteDeadline(time.Now().Add(time.Duration(*expire) * time.Second))
			_, err = to.Write(bytes[:read])
			if err != nil {
				complete <- true
				done <- true
				return
			}
		}
	}
}
