package Bst

type Interface interface {
	New() Interface
	Insert(item int)
	Delete(item int)
	Exists(item int) bool
	ShowInOrder()
	ShowPreOrder()
	ShowPostOrder()
}
