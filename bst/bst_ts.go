package Bst

import (
	"sync"
)

type BstTS struct {
	Bst
	l sync.RWMutex
}

// Construtor
func NewBstTs() *BstTS {
	bst := &BstTS{}

	// Ensure interface compliance
	var _ Interface = bst
	return bst
}

func (bst *BstTS) New() Interface {
	return NewBstTs()
}

func (b *BstTS) Insert(val int) {
	b.l.Lock()
	defer b.l.Unlock()

	if b.root == nil {
		n := addNode(nil, val)
		b.root = n
	} else {
		b.insert(b.root, nil, val)
	}
	return
}

func (b *BstTS) insert(root *Node, parent *Node, val int) *Node {
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

func (b *BstTS) delete(root *Node, val int) (n *Node) {
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

func (b *BstTS) Delete(val int) {
	b.l.Lock()
	defer b.l.Unlock()

	if b.root == nil {
		return
	} else {
		b.delete(b.root, val)
	}
}

func (b *BstTS) Exists(item int) bool {
	b.l.RLock()
	defer b.l.RUnlock()
	return search(b.root, item)
}

func (b *BstTS) ShowInOrder() {
	b.l.RLock()
	defer b.l.RUnlock()
	showInOrder(b.root)
}

func (b *BstTS) ShowPreOrder() {
	b.l.RLock()
	defer b.l.RUnlock()
	showPreOrder(b.root)
}

func (b *BstTS) ShowPostOrder() {
	b.l.RLock()
	defer b.l.RUnlock()
	showPostOrder(b.root)
}
