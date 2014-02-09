//理解深度优先搜索，和广度优先搜索
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stack>
#include <queue>
using namespace std;

#define Element char
#define format "%c"

typedef struct Node
{
	/* data */
	Element data;
	struct Node *lchild;
	struct Node *rchild;
} *Tree;

int index = 0;
void treeNodeConstructor(Tree &root, Element data[]) {
	Element = data[index++];
	if (e == '#') {
		root = NULL;
	} else {
		root = (Node *) malloc(sizeof(Node));
		root -> data = e;
		treeNodeConstructor(root->lchild, data);
		treeNodeConstructor(root->rchild, data);
	}
}

void depthFirstSearch(Tree root) {
	stack<Node *> nodeStack;
	nodeStack.push(root);
	Node *node;
	while(!nodeStack.empty()) {
		node = nodeStack.top();
		printf(format, node->data);
		nodeStack.pop();
		if (node->rchild) {
			nodeStack.push(node->rchild);
		}
		if (node->lchild) {
			nodeStack.push(node->lchild);
		}
	}
}

void breadthFirstSearch(Tree root) {
	queue<Node *> nodeQueue;
	nodeQueue.push(root);
	Node *node;
	while (!nodeQueue.empty()) {
		node = nodeQueue.front();
		nodeQueue.pop();
		printf(format, node->data);
		if (node->lchild) {
			nodeQueue.push(node->lchild);
		}
		if (node->rchild) {
			nodeQueue.push(node->rchild);
		}
	}
}

int main() {

    //上图所示的二叉树先序遍历序列,其中用'#'表示结点无左子树或无右子树
    Element data[15] = {'A', 'B', 'D', '#', '#', 'E', '#', '#', 'C', 'F','#', '#', 'G', '#', '#'};
    Tree tree;
    treeNodeConstructor(tree, data);
    printf("深度优先遍历二叉树结果: ");
    depthFirstSearch(tree);
    printf("\n\n广度优先遍历二叉树结果: ");
    breadthFirstSearch(tree);
    return 0;
}