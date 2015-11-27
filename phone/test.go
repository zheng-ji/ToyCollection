package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io/ioutil"
	//"os"
)

//type header struct {
//version []byte
//offset  int
//}

func main() {
	//version
	f, _ := ioutil.ReadFile("phone.dat")
	fmt.Println(string(f[0:4]))
	fmt.Println(len(f))

	//a := new(header)
	//buf := new(bytes.Buffer)
	//binary.Read(f[0:8], binary.LittleEndian, &a)
	//fmt.Println(a)
	//fp, _ := os.Open("phone.dat")
	//buf := make([]byte, 4)
	//fp.Read(buf)
	//var k []byte
	//first offset
	var k int32
	buffer := bytes.NewBuffer(f[4:8])
	binary.Read(buffer, binary.LittleEndian, &k)
	fmt.Println(k)

	// 记录数目
	total := (int32(len(f)) - k) / 9
	fmt.Println(total)
}
