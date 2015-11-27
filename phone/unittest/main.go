package main

import (
    "fmt"
	"phone/unittest/phone"
)

func main() {
	phone.Load("phone.dat")
    phone.Display()
	/*fmt.Println(phone.Find("15920554608"))*/
    /*pr := phone.Find("13580198235")*/
    fmt.Println("=====")
    /*fmt.Println(pr.PhoneNum)*/
    /*fmt.Println(pr.AreaZone)*/
    /*fmt.Println(pr.CardType)*/
    /*fmt.Println(pr.City)*/
    /*fmt.Println(pr.ZipCode)*/
    /*fmt.Println(pr.Province)*/
}
