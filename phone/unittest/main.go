package main

import (
    "fmt"
	"phone/unittest/phone"
    "time"
)

func Since(t time.Time) int {
    return int(time.Since(t).Nanoseconds() / 1e6)
}

func main() {
    timeStart := time.Now()
	phone.Load("phone.dat")
    fmt.Println(Since(timeStart))
    fmt.Println("=====")
    timeStart = time.Now()
    phone.Display()
    fmt.Println(Since(timeStart))
    fmt.Println("=====")
    timeStart = time.Now()
    pr := phone.Find("13580198235")
    fmt.Println(pr.PhoneNum)
    fmt.Println(pr.AreaZone)
    fmt.Println(pr.CardType)
    fmt.Println(pr.City)
    fmt.Println(pr.ZipCode)
    fmt.Println(pr.Province)
    fmt.Println(Since(timeStart))
    fmt.Println("=====")
    timeStart = time.Now()
    pr = phone.Find("1581381745")
    fmt.Println(pr.PhoneNum)
    fmt.Println(pr.AreaZone)
    fmt.Println(pr.CardType)
    fmt.Println(pr.City)
    fmt.Println(pr.ZipCode)
    fmt.Println(pr.Province)
    fmt.Println(Since(timeStart))
    fmt.Println("=====")
}
