#ifndef BINTREE_H
#define BINTREE_H
#include <iostream>
#include <vector>
#include <deque>
using namespace std;

class TreeNode{
    public:
        TreeNode (int value){
            m_nValue = value;
            m_pLeft = m_pRight = NULL;
        };
        int m_nValue;
        TreeNode * m_pLeft;
        TreeNode * m_pRight;
};

class BinTree{
    public:
        BinTree() {
            pHead = NULL;
        }
        void CreateBinTree(int value);
        void PreDisplay(TreeNode * pHead);
        void MidDisplay(TreeNode * pHead);
        void PosDisplay(TreeNode * pHead);
        TreeNode* GetHead();
        int NumberOfElement();
        int depth(TreeNode * pHead);
        bool IsBalanced(TreeNode * pHead);//是否是平衡树
        void FindPath(int expectedSum);
        void FindPath(TreeNode *pHead,int expectedSum,std::vector<int>&path,int & currentSum);
        void PrintFromTopToBottom();
    private:
        TreeNode* pHead;
        int static counter;
};
#endif
