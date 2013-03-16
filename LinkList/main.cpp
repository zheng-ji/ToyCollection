#include"LinkClass.h"

ListNode * merge(ListNode*pHead1,ListNode*pHead2) {
    if (pHead1 == NULL) 
        return pHead1;
    if (pHead2 == NULL)
        return pHead2;

    ListNode * pMergedHead = NULL;
    if (pHead1->m_nValue < pHead2->m_nValue) {
        pMergedHead = pHead1;
        pMergedHead ->m_pNext = merge(pHead1->m_pNext,pHead2);
    } else {
        pMergedHead = pHead2;
        pMergedHead ->m_pNext = merge(pHead1,pHead2->m_pNext);
    }
    return pMergedHead;
}

void display(ListNode * pHead) {
    while(pHead != NULL) {
        cout << pHead->m_nValue;
        pHead = pHead->m_pNext;
    }
    cout << endl;
}

int main() {
    /*
    List linklist;
    int input [5] = {3,7,2,1,4};
    for (int i = 0; i < 5; i++) {
        linklist.AddToTail(input[i]);
    }
    linklist.display();
    cout << "---after delete Node---\n "<<endl;
    linklist.RemoveNode (1);
    linklist.display();
    cout << "\n---Reverse with stack Node---\n"<<endl;
    linklist.ReverseDisplayWithStack();
    cout << "\n---Reverse with Recursive Node---\n"<<endl;
    linklist.ReverseDisplayWithRecursive(linklist.GetHead());
    cout << "\n---After Reverse List---\n"<<endl;
    linklist.display();
    cout << "\n---Last Kth Elment List---\n"<<endl;
    cout << (linklist.FindKthToTail(2))->m_nValue;
    cout << "\n---Asc Sort---\n"<<endl;
    linklist.AscSort();
    linklist.display();
    return 0;
    */

    List linklist_1,linklist_2;
    int input_1 [5] = {3,7,2,1,4};
    for (int i = 0; i < 5; i++) {
        linklist_1.AddToTail(input_1[i]);
    }
    int input_2 [5] = {9,0,6,5,8};
    for (int i = 0; i < 5; i++) {
        linklist_2.AddToTail(input_2[i]);
    }
    display(merge(linklist_1.GetHead(),linklist_2.GetHead()));
    return 0;
}
