package main

import (
	//"fmt"
	"searchPhone/phone/unittest/phone"
)

func main() {
	phone.Load("phone.dat")
	phone.Display()
	phone.Find("15920554608")
}
