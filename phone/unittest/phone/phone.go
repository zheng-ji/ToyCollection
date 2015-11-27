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
    INT_LEN = 4
    CHAR_LEN = 1
	HEAD_LENGTH        = 8
	PHONE_INDEX_LENGTH = 9
    CHUNK = 100
)


var content []byte

func Load(phone_dat string) {
	var err error
	content, err = ioutil.ReadFile(phone_dat)
	if err != nil {
		log.Fatal(err)
	}
}

func Display() {
	fmt.Println(getVersion())
	fmt.Println(getTotalRecord())
	fmt.Println(getFirstRecordOffset())
}

func getVersion() string {
	return string(content[0:INT_LEN])
}

func getTotalRecord() int32 {
	total := (int32(len(content)) - getFirstRecordOffset()) / PHONE_INDEX_LENGTH
	return total
}

func getFirstRecordOffset() int32 {
	var offset int32
	buffer := bytes.NewBuffer(content[INT_LEN:INT_LEN * 2])
	binary.Read(buffer, binary.LittleEndian, &offset)
	return offset
}

func getIndexRecord(offset int32) (phone_prefix int32, record_offset int32, card_type byte) {
    buffer := bytes.NewBuffer(content[offset: offset+INT_LEN])
    binary.Read(buffer, binary.LittleEndian, &phone_prefix)
    buffer = bytes.NewBuffer(content[offset+INT_LEN: offset+INT_LEN * 2])
    binary.Read(buffer, binary.LittleEndian, &record_offset)
    buffer = bytes.NewBuffer(content[offset+INT_LEN*2: offset+INT_LEN*2+CHAR_LEN])
    binary.Read(buffer, binary.LittleEndian, &card_type)
    /*fmt.Println("----")*/
    /*fmt.Println(phone_prefix)*/
    /*fmt.Println(record_offset)*/
    /*fmt.Println(card_type)*/
    return
}

type PhoneRecord struct {
    PhoneNum string
    Province string
    City    string
    ZipCode string
    AreaZone string
    CardType string
}

func getOpCompany(cardtype byte) string{
    var card_str = ""
    switch cardtype {
    case '1':
        card_str = "中国移动"
        break
    case '2':
        card_str = "中国联通"
        break
    case '3':
        card_str = "中国电信"
        break
    case '4':
        card_str = "中国虚拟电信"
        break
    case '5':
        card_str = "中国虚拟联通"
        break
    default:
        card_str = "中国虚拟移动"
        break
    }
    return card_str
}

func Find(phone_num string) (pr *PhoneRecord) {
	if len(phone_num) < 7 || len(phone_num) > 11 {
		log.Fatal("illegal phone length")
		return 
	}

	var left int32 = 0
	phone_seven_int, _ := strconv.ParseInt(phone_num[0:7], 10, 32)
    phone_seven_int32 := int32(phone_seven_int)
    total_len := int32(len(content))
	right := getTotalRecord()
    firstPhoneRecordOffset := getFirstRecordOffset()
	for {
		if left > right {
			break
		}
        mid := (left + right) / 2
        current_offset := firstPhoneRecordOffset + mid * PHONE_INDEX_LENGTH

		if current_offset >= total_len {
			break
		}
        cur_phone, record_offset, card_type := getIndexRecord(current_offset)
		if cur_phone > phone_seven_int32 {
			right = mid - 1
		} else if cur_phone < phone_seven_int32 {
			left = mid + 1
		} else {
            s := record_offset
            e := record_offset + int32(strings.Index(string(content[record_offset:record_offset+CHUNK]), "\000"))
            record_content := string(content[s:e])
            _tmp := strings.Split(record_content, "|")
            card_str := getOpCompany(card_type)
            pr = &PhoneRecord {
                PhoneNum: phone_num,
                Province: _tmp[0],
                City: _tmp[1],
                ZipCode: _tmp[2],
                AreaZone: _tmp[3],
                CardType: card_str,
            }
            return
		}
	}
	return 
}
