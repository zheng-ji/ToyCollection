#ifndef LINKCLASS_H
#define LINKCLASS_H
#include<iostream>
#include<stack>
using namespace std;

class ListNode {
    public:
        ListNode(int value) {
            m_nValue = value;
            m_pNext = NULL;
        };
        /*
        ~ListNode () {
            delete m_pNext;
        };
        */
        int m_nValue;
        ListNode * m_pNext;
};

class List {
    public:
        List() {
            pHead = NULL;
        }
        ~List();
        void display();
        void AddToTail(int value);
        void RemoveNode(int value);
        void ReverseDisplayWithStack();
        void ReverseDisplayWithRecursive(ListNode * pHead);
        void ReverseList();
        ListNode* GetHead();
        void AscSort();
        ListNode* FindKthToTail(int k);
    private:
        ListNode * pHead;
        int static counter;
};

#endif
