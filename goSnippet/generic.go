package main

import (
	"errors"
	"fmt"
)

// use Vars
var (
	ERR_ELEM_EXIST    = errors.New("exists")
	ERR_ELEM_NT_EXIST = errors.New("not exist")
)

type Student struct {
	id   string
	name string
}

func (this Student) GetId() string {
	return this.id
}

func (this Student) IsEqual(obj interface{}) bool {
	if student, ok := obj.(Student); ok {
		return this.GetId() == student.GetId()
	}
	return false
}

type Comparable interface {
	isEqual(obj interface{}) bool
}

func IsEqual(a, b interface{}) bool {
	if comparable, ok := a.(Comparable); ok {
		return comparable.isEqual(b)
	} else {
		return a == b
	}
}

type Slice []interface{}

func NewSlice() Slice {
	return make(Slice, 0)
}

func (this *Slice) Add(elem interface{}) error {
	for _, v := range *this {
		if IsEqual(v, elem) {
			fmt.Printf("Slice:Add elem:%v alreay exist\n", elem)
			return ERR_ELEM_EXIST
		}
	}
	*this = append(*this, elem)
	fmt.Printf("slice:Add elem:%v succ\n", elem)
	return nil
}

func (this *Slice) Remove(elem interface{}) error {
	found := false
	for i, v := range *this {
		if IsEqual(v, elem) {
			if i == len(*this)-1 {
				*this = (*this)[:i]
			} else {
				// append 支持可变变量
				*this = append((*this)[:i], (*this)[i+1:]...)
			}
			found = true
			break
		}
	}
	if !found {
		fmt.Printf("Slice:Remove elem:%v not exist\n", elem)
		return ERR_ELEM_NT_EXIST
	}
	fmt.Printf("Slice:Remove elem:%v succ\n", elem)
	return nil
}

func struceSliceExec() {
	xiaoMing := Student{"1001", "xiaoming"}
	xiaoLei := Student{"1001", "xiaolei"}
	xiaoFang := Student{"1001", "xiao fang"}

	slice := NewSlice()

	slice.Add(xiaoMing)
	fmt.Println("current", slice)

	slice.Add(xiaoLei)
	fmt.Println("current", slice)

	slice.Add(xiaoLei)
	fmt.Println("current", slice)

	slice.Add(xiaoFang)
	fmt.Println("current", slice)

	slice.Remove(xiaoLei)
	fmt.Println("current", slice)

	slice.Remove(xiaoLei)
	fmt.Println("current", slice)

	slice.Remove(xiaoFang)
	fmt.Println("current", slice)
}

func main() {
	struceSliceExec()
}
