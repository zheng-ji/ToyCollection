#include "BinTree.h"

bool DoesTree1HaveTree2(TreeNode*pRoot1,TreeNode*pRoot2) {
    if (pRoot2 == NULL) 
        return true;
    if (pRoot1 == NULL) 
        return false;
    if (pRoot1->m_nValue != pRoot2->m_nValue) {
        return false;
    }
    return DoesTree1HaveTree2(pRoot1->m_pLeft,pRoot2->m_pLeft) && DoesTree1HaveTree2(pRoot1->m_pRight,pRoot2->m_pRight);
}

bool HasSubtree(TreeNode* pRoot1,TreeNode* pRoot2) {
    bool result = false;
    if (pRoot1 != NULL && pRoot2 != NULL) {
        if (pRoot1->m_nValue == pRoot2->m_nValue) {
            result = DoesTree1HaveTree2(pRoot1,pRoot2);
        }
        if (!result)
            HasSubtree(pRoot1->m_pLeft,pRoot2);
        if (!result)
            HasSubtree(pRoot1->m_pRight,pRoot2);
    }
    return result;
}


int main() {
    BinTree bt;
    int input[5] = {3,5,1,4,2};
    for (int i = 0; i < 5 ; i++) {
        bt.CreateBinTree(input[i]);
    }
    cout << "\n----pre display-----\n";
    bt.PreDisplay(bt.GetHead());
    cout << "\n----mid display-----\n";
    bt.MidDisplay(bt.GetHead());
    cout << "\n----pos display-----\n";
    bt.PosDisplay(bt.GetHead());
    cout << "\n----the number of elments----\n";
    cout << bt.NumberOfElement();
    cout << "\n----the depth of BinaryTree----\n";
    cout << bt.depth(bt.GetHead());
    cout << "\n----Is the BinaryTree Balanced?----\n";
    if (bt.IsBalanced(bt.GetHead()))
        cout <<"Y";
    else 
        cout << "N";
    cout << "\n----Find Path 6----\n";
    bt.FindPath(6);
    cout << "\n----Print From Top To Bottom----\n";
    bt.PrintFromTopToBottom();
    return 0;
    /*
    BinTree bt1;
    int input1[5] = {3,5,1,4,2};
    for (int i = 0; i < 5 ; i++) {
        bt1.CreateBinTree(input1[i]);
    }
    BinTree bt2;
    int input2[5] = {3,5,1};
    for (int i = 0; i < 3 ; i++) {
        bt2.CreateBinTree(input2[i]);
    }
    if (HasSubtree(bt1.GetHead(),bt2.GetHead())) {
        cout << "bt2 is the subtree of bt1";
    }
    return 0;
    */
}
