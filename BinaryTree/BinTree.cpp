#include "BinTree.h"
int BinTree::counter = 0;

void BinTree::CreateBinTree(int value) {
    TreeNode * pNew = new TreeNode(value);
    if (pHead == NULL) {
        pHead = pNew;
    } else {
        TreeNode * pNode = pHead;
        TreeNode * pTemp = pHead;
        while(pNode != NULL) {
            if (pNode->m_nValue >= value) {
                pTemp = pNode;
                pNode = pNode ->m_pLeft;
            } else {
                pTemp = pNode;
                pNode = pNode ->m_pRight;
            }
        }
        if (pTemp->m_nValue < value) {
            pTemp->m_pRight = pNew;
        }
        if (pTemp->m_nValue > value) {
            pTemp->m_pLeft = pNew;
        }
    }
    counter ++ ;
}

TreeNode* BinTree:: GetHead() {
    return pHead;
}

void BinTree::PreDisplay(TreeNode * pHead) {
    cout << pHead->m_nValue << " ";
    if (pHead->m_pLeft != NULL)
        PreDisplay(pHead->m_pLeft);
    if (pHead->m_pRight != NULL)
        PreDisplay(pHead->m_pRight);
}

void BinTree::MidDisplay(TreeNode * pHead) {
    if (pHead->m_pLeft != NULL)
        PreDisplay(pHead->m_pLeft);
    cout << pHead->m_nValue << " ";
    if (pHead->m_pRight != NULL)
        PreDisplay(pHead->m_pRight);
}

void BinTree::PosDisplay(TreeNode * pHead) {
    if (pHead->m_pLeft != NULL)
        PreDisplay(pHead->m_pLeft);
    if (pHead->m_pRight != NULL)
        PreDisplay(pHead->m_pRight);
    cout << pHead->m_nValue << " ";
}

int BinTree::NumberOfElement() {
    return counter;
}

int BinTree::depth(TreeNode * pHead) {
    if (pHead == NULL)
        return 0;
    int left = depth(pHead->m_pLeft);
    int right = depth(pHead->m_pRight);
    return (left > right)? (left + 1): (right + 1);
}

bool BinTree::IsBalanced(TreeNode *pHead) {
    if (pHead == NULL)
        return false;
    int left = depth(pHead ->m_pLeft);
    int right = depth(pHead ->m_pRight);
    int diff = right - left;
    if (diff > 1 || diff < -1)
        return false;
    return IsBalanced(pHead->m_pLeft) && IsBalanced(pHead->m_pRight);
}

void BinTree::FindPath(int expectedSum) {
    if (pHead == NULL)
        return ;
    std::vector<int> path;
    int currentSum = 0;
    FindPath (pHead,expectedSum,path,currentSum);
}

void BinTree::FindPath(TreeNode *pHead,int expectedSum,std::vector<int> &path,int &currentSum) {
    if (pHead == NULL)
        return;

    currentSum += pHead->m_nValue;
    path.push_back(pHead->m_nValue);

    bool isLeaf = (pHead->m_pLeft == NULL) && (pHead->m_pRight == NULL);
    if (currentSum == expectedSum && isLeaf) {
        std::vector<int>::iterator iter = path.begin();
        for (;iter != path.end();++iter) {
            cout << *iter <<"\t";
        }
        cout << endl;
    }

    if (pHead ->m_pLeft != NULL)
        FindPath(pHead->m_pLeft,expectedSum,path,currentSum);
    if (pHead ->m_pRight != NULL)
        FindPath(pHead->m_pRight,expectedSum,path,currentSum);
    path.pop_back();
}

void BinTree::PrintFromTopToBottom() {
    if (pHead == NULL)
        return ;
    std::deque<TreeNode *> dequeTreeNode;
    dequeTreeNode.push_back(pHead);
    while(dequeTreeNode.size()) {

        TreeNode * pNode = dequeTreeNode.front();
        dequeTreeNode.pop_front();
        cout << pNode->m_nValue;

        if (pNode->m_pLeft != NULL)
            dequeTreeNode.push_back(pNode->m_pLeft);
        if (pNode->m_pRight != NULL)
            dequeTreeNode.push_back(pNode->m_pRight);
    }
}
