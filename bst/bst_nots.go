package Bst

import (
	"fmt"
)

type Node struct {
	left   *Node
	right  *Node
	parent *Node
	value  int
}

type Bst struct {
	root *Node
}

type BstNonTS struct {
	Bst
}

// Construtor
func NewBstNonTs() *BstNonTS {
	bst := new(BstNonTS)

	// Ensure interface compliance
	var _ Interface = bst
	return bst
}

func (b *BstNonTS) New() Interface {
	return NewBstNonTs()
}

func addNode(parent *Node, val int) *Node {
	return &Node{nil, nil, parent, val}
}

func (b *BstNonTS) Insert(val int) {
	if b.root == nil {
		n := addNode(nil, val)
		b.root = n
	} else {
		b.insert(b.root, nil, val)
	}
	return
}

func (b *BstNonTS) insert(root *Node, parent *Node, val int) *Node {
	switch {
	case root == nil:
		return addNode(parent, val)
	case val <= root.value:
		root.left = b.insert(root.left, root, val)
	case val > root.value:
		root.right = b.insert(root.right, root, val)
	}
	return root
}

func (b *BstNonTS) Delete(val int) {
	if b.root == nil {
		return
	} else {
		b.delete(b.root, val)
	}
}

func (b *BstNonTS) delete(root *Node, val int) (n *Node) {
	if b.root.value == val && b.root.left == nil && b.root.right == nil {
		n = b.root
		b.root = nil
		return
	} else if root.value != val {
		if val <= root.value {
			b.delete(root.left, val)
		} else if val > root.value {
			b.delete(root.right, val)
		}
	} else if root.value == val && root.left == nil && root.right == nil {
		n = b.root
		b.root = nil
		return
	} else if root.value == val && (root.left == nil || root.right == nil) {
		// there's a better, cleaner way to do this in one line
		var n1 *Node
		if root.left != nil {
			n1 = root.left
		} else {
			n1 = root.right
		}
		switch {
		case root.parent.left == root:
			root.parent.left = n1
			return root
		case root.parent.right == root:
			root.parent.right = n1
			return root
		}
	} else if root.value == val && root.left != nil && root.right != nil {
		// Needs to be implemented later
	}
	return nil
}

func search(root *Node, value int) bool {
	if root != nil {
		if value == root.value {
			return true
		} else if value > root.value {
			return search(root.right, value)
		} else {
			return search(root.left, value)
		}
	}
	return false
}

func (b *BstNonTS) Exists(item int) bool {
	return search(b.root, item)
}

func showInOrder(root *Node) {
	if root != nil {
		showInOrder(root.left)
		fmt.Println(root.value)
		showInOrder(root.right)
	}
}

func (b *BstNonTS) ShowInOrder() {
	showInOrder(b.root)
}

func showPreOrder(root *Node) {
	if root != nil {
		fmt.Println(root.value)
		showPreOrder(root.left)
		showPreOrder(root.right)
	}
}

func (b *BstNonTS) ShowPreOrder() {
	showPreOrder(b.root)
}

func showPostOrder(root *Node) {
	if root != nil {
		fmt.Println(root.value)
		showInOrder(root.left)
		showInOrder(root.right)
	}
}

func (b *BstNonTS) ShowPostOrder() {
	showPostOrder(b.root)
}
