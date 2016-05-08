package Bst

import (
	"testing"
)

func TestSetNonTS_All(t *testing.T) {
	s := NewBstNonTs()
	s.Insert(1)
	s.Insert(2)
	s.Insert(9)

	s.Exists(9)
	s.Delete(2)

	s.ShowInOrder()
	s.ShowPreOrder()
	s.ShowPostOrder()
}

func TestSetTS_All(t *testing.T) {
	s := NewBstTs()
	s.Insert(8)
	s.Insert(4)
	s.Insert(5)

	s.Exists(4)

	s.ShowInOrder()
	s.ShowPreOrder()
	s.ShowPostOrder()
}
