#include"LinkClass.h"
int List::counter = 0;

List::~List() {
    ListNode *pNode = NULL;
    while(pHead != NULL) {
        pNode = pHead;
        delete pNode;
        pHead = pHead->m_pNext;
    }
}

void List::AddToTail(int value) {
    ListNode  *pNew = new ListNode (value);
    if (pHead == NULL) {
        pHead = pNew;
    } else {
        ListNode * pNode = pHead;
        while (pNode -> m_pNext != NULL) {
            pNode = pNode ->m_pNext;
        }
        pNode->m_pNext = pNew;
    }
    counter++;
}

void List::display() {
    ListNode * pNode = pHead;
    while (pNode != NULL) {
        cout << pNode->m_nValue<< " ";
        pNode = pNode->m_pNext;
    }
    cout << "Elements"<<counter;
    cout << endl;
}

void List::RemoveNode (int value) {
    if (pHead == NULL)
        return;
    ListNode * pToBeDeleted = NULL;
    if (pHead->m_nValue == value) {
        pToBeDeleted = pHead;
        pHead = pHead->m_pNext;
    } else {
        ListNode *pNode = pHead;
        while (pNode->m_pNext-> m_nValue != value && pNode->m_pNext != NULL) {
            pNode = pNode->m_pNext;
        }
        if (pNode ->m_pNext != NULL && pNode ->m_pNext->m_nValue == value) {
            pToBeDeleted = pNode->m_pNext;
            pNode ->m_pNext = pNode ->m_pNext ->m_pNext;
        }
    }
    if (pToBeDeleted != NULL) {
        delete pToBeDeleted;
        pToBeDeleted = NULL;
        counter -- ;
    }
}

void List::ReverseDisplayWithStack() {
    stack<ListNode *> node_stack;
    ListNode *pNode = pHead;
    while(pNode != NULL) {
        node_stack.push(pNode);
        pNode = pNode->m_pNext;
    }
    while(!node_stack.empty()) {
        pNode = node_stack.top();
        cout << pNode->m_nValue<< " ";
        node_stack.pop();
    }
    cout << endl;
}

void List::ReverseDisplayWithRecursive(ListNode * pHead) {
    if (pHead != NULL) {
        if (pHead->m_pNext != NULL) {
            ReverseDisplayWithRecursive(pHead->m_pNext);
        } 
    }
    cout << pHead->m_nValue << " ";
}

void List::ReverseList() {
    ListNode* pNext = NULL;
    ListNode* pReverseHead = NULL;
    ListNode* pPrev = NULL;
    ListNode* pNode = pHead;
    while(pNode != NULL) {
        pNext = pNode->m_pNext;
        if (pNext == NULL)
            pReverseHead = pNode;
        pNode ->m_pNext = pPrev;
        pPrev = pNode;
        pNode = pNext;
    }
    pHead = pReverseHead;
}

ListNode * List::GetHead () {
    return this->pHead;
}

ListNode * List::FindKthToTail(int k) {
    int i = 0;
    ListNode * pNode = pHead;
    for (;i < k; i++) {
        pNode = pNode ->m_pNext;
    }
    ListNode *pLastKthNode = pHead;
    while(pNode != NULL) {
        pNode = pNode ->m_pNext;
        pLastKthNode = pLastKthNode ->m_pNext;
    }
    return pLastKthNode;
}

void List::AscSort() {
    int tmp = 0;
    ListNode* pNode = pHead;
    while(pNode != NULL) {
        ListNode * tempNode = pNode->m_pNext;
        while(tempNode != NULL) {
            if (pNode->m_nValue > tempNode ->m_nValue) {
                tmp = pNode->m_nValue;
                pNode->m_nValue = tempNode->m_nValue;
                tempNode->m_nValue = tmp;
            }
            tempNode = tempNode ->m_pNext;
        }
        pNode = pNode->m_pNext;
    }
}
