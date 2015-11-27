package phone

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io/ioutil"
	"log"
	"strconv"
	"strings"
)

const (
	HEAD_LENGTH        = 8
	PHONE_INDEX_LENGTH = 9
	DAT_FILE           = "phone.dat"
)

var content []byte
var Version string
var FirstPhoneRecordOffset int32
var TotalRecord int32

type IndexRecord struct {
	phone_prefix  int32
	record_offset int32
	card_type     byte
}

func Load(phone_dat string) {
	var err error
	content, err = ioutil.ReadFile(phone_dat)
	if err != nil {
		log.Fatal(err)
	}
	Version = getVersion()
	TotalRecord = getTotalRecord()
	FirstPhoneRecordOffset = getFirstRecordOffset()
}

func Display() {
	fmt.Println(Version)
	fmt.Println(TotalRecord)
	fmt.Println(FirstPhoneRecordOffset)
}

func getVersion() string {
	return string(content[0:4])
}

func getTotalRecord() int32 {
	total := (int32(len(content)) - FirstPhoneRecordOffset) / PHONE_INDEX_LENGTH
	return total
}

func getFirstRecordOffset() int32 {
	var offset int32
	buffer := bytes.NewBuffer(content[4:8])
	binary.Read(buffer, binary.LittleEndian, &offset)
	return offset
}

func getIndexRecord(offset int32) (phone_prefix int32, record_offset int32) {
	ir := new(IndexRecord)
	buffer := bytes.NewBuffer(content[offset : offset+PHONE_INDEX_LENGTH])
	binary.Read(buffer, binary.LittleEndian, &ir)
	fmt.Println("----")
	fmt.Println(ir.phone_prefix)
	fmt.Println(ir.record_offset)
	fmt.Println(ir.card_type)
	return ir.phone_prefix, ir.record_offset
}

func Find(phone_num string) string {
	if len(phone_num) < 7 || len(phone_num) > 11 {
		log.Fatal("illegal phone length")
		return ""
	}

	phone_seven_int, _ := strconv.ParseInt(phone_num[0:7], 10, 32)
	fmt.Println(phone_seven_int)

	var left int32 = 0
	var mid int32 = 0
	var current_offset int32 = 0
	right := TotalRecord
	var cur_phone int32
	var record_offset int32
	var s int32 = 0
	var e int32 = 0
	record_content := ""
	for {
		if left > right {
			break
		}
		mid = (left + right) / 2
		current_offset = FirstPhoneRecordOffset + mid*PHONE_INDEX_LENGTH

		if current_offset >= int32(len(content)) {
			break
		}
		cur_phone, record_offset = getIndexRecord(current_offset)
		if cur_phone > int32(phone_seven_int) {
			right = mid + 1
		} else if cur_phone < int32(phone_seven_int) {
			left = mid + 1
		} else {
			s = record_offset
			e = record_offset + int32(strings.Index(string(content[record_offset:int32(len(content))]), "\000"))
			record_content = string(content[s:e])
			return record_content
		}
	}
	return ""
}
