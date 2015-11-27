package phone

import {
	"fmt"
	"os"
	"io/ioutil"
}

const {
	HEAD_LENGTH = 8
	PHONE_INDEX_LENGTH = 9
}

type Phone struct {
	filePath					string
	version						string
	firstPhoneRecordOffset		int32
	content				[]byte
	totalRecord			int32
}

var defaultPhone *Phone

func init() {
	defaultPhone := new(Phone)
	defaultPhone.filePath = "phone.dat"
	//FIXME handle error
	defaultPhone.content, _ = ioutil.ReadFile(defaultPhone.filePath)
	defaultPhone.version = getVersion()
	defaultPhone.firstPhoneRecordOffset = getFirstRecordOffset()
	defaultPhone.totalRecord = 
}

func getVersion() string {
	return string(defaultPhone.content[0:4])
}

func getTotalRecord() int32 {
	total := (int32(len(defaultPhone.content)) - defaultPhone.version) / PHONE_INDEX_LENGTH
	return total
}

func getFirstRecordOffset() int32 {
	var version int32
	buffer := bytes.NewBuffer(defaultPhone.content[4:8])
	binary.Read(buffer, binary.LittleEndian, &k)
	return version
}
