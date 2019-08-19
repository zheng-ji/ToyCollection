package main

import (
	dis "discovery"
	"fmt"
	"log"
	"time"
)

func main() {

	serviceName := "s-test"
	serviceInfo := dis.ServiceInfo{IP: "127.0.0.1"}

	s, err := dis.NewService(serviceName, serviceInfo, []string{
		"http://127.0.0.1:12379",
		"http://127.0.0.1:22379",
		"http://127.0.0.1:32379",
	})

	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("name:%s, ip:%s\n", s.Name, s.Info.IP)

	go func() {
		time.Sleep(time.Second * 20)
		s.Stop()
	}()

	s.Start()
}
