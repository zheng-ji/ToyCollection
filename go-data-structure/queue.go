package main

import (
	"container/list"
	"fmt"
)

type Queue struct {
	data *list.List
}

func NewQueue() *Queue {
	q := new(Queue)
	q.data = list.New()
	return q
}

func (q *Queue) Enqueue(v interface{}) {
	q.data.PushBack(v)
}

func (q *Queue) Dequeue() interface{} {
	iter := q.data.Front()
	v := iter.Value
	q.data.Remove(iter)
	return v
}

func (q *Queue) Dump() {
	for iter := q.data.Front(); iter != nil; iter = iter.Next() {
		fmt.Println("item:", iter.Value)
	}
}

func main() {
	q := NewQueue()
	q.Enqueue("one")
	q.Enqueue("two")
	q.Dump()
	v := q.Dequeue()
	fmt.Println("v:", v)
	q.Dump()
}
