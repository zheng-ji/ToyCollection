package main

import (
	dis "discovery"
	"fmt"
	"log"
	"time"
)

func main() {

	m, err := dis.NewMaster([]string{
		"http://127.0.0.1:12379",
		"http://127.0.0.1:22379",
		"http://127.0.0.1:32379"}, "services/")
	if err != nil {
		log.Fatal(err)
	}

	for {
		for k, v := range m.Nodes {
			fmt.Printf("node:%s, ip=%s\n", k, v.Info.IP)
		}
		fmt.Printf("nodes num = %d\n", len(m.Nodes))
		time.Sleep(time.Second * 5)
	}
}
