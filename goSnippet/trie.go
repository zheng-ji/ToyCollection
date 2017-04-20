package main

import (
    "fmt"
)

type Node struct {
    is_end           bool
    element          byte 
    prefix_count     int 
    children         [26]*Node
}

type Trie struct {
    head *Node
}

func (trie *Trie) insert(word string) {
    current := trie.head
    count := 0
    for _, v := range word {
        index := v - 'a'
        if nil == current.children[index] {
            current.children[index] = new(Node)
            current = current.children[index]
            count += 1
            current.prefix_count = count
        } else {
            current = current.children[index]
        }
    }
    current.is_end = true
}

func (trie *Trie) search(word string) bool {
    current := trie.head
    for _, v:= range word {
        index := v - 'a'
        if nil == current.children[index] {
            return false
        } else {
            current = current.children[index]
        }
    }
    return current.is_end
}

func main() {
    trie := new(Trie)
    if trie.head == nil {
        fmt.Println("hello")
        trie.head = new(Node)
    }
    fmt.Println(trie.head.children)

    if nil != trie.head.children[5] {
        fmt.Println("element is not null")
    }

    if nil == trie.head.children[5] {
        fmt.Println("element is null")
    }
    trie.insert("hello")
    trie.insert("world")
    if true == trie.search("hell") {
        fmt.Println("good")
    }
}
