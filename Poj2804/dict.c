#include<stdio.h>
#include<string.h>
#include<algorithm>

#define MAX_WORD_LEN 11
#define MAX_DICTION_ITEM (10000+10)

using std::sort;

struct Dictionary {
    char szWord[MAX_WORD_LEN];
    char szEnglish[MAX_WORD_LEN];
};

Dictionary diction[MAX_DICTION_ITEM];

bool CmpDictionitem(Dictionary one,Dictionary two){
    return strcmp(one.szWord,two.szWord) < 0;
}

int FindEnglish(char *pszWord,int nItemNum) {
    int nBeg = 0,nEnd = nItemNum -1;
    while (nBeg <= nEnd) {
        int nMid = (nBeg + nEnd) / 2;
        int nCmp = 0;
        nCmp = strcmp(pszWord,diction[nMid].szWord);
        if (nCmp == 0)
            return nCmp;
        else if (nCmp < 0) {
            nEnd = nMid - 1;
        } else {
            nBeg = nMid + 1;
        }
    }
    return -1;
}

int main () {
    char szStr[30];
    char szWord[MAX_WORD_LEN];
    int nCount = 0;
    int nAnsitem = 0;
    while (fgets(szStr,29,stdin),szStr[0] != '\n') {
        sscanf(szStr,"%s%s",diction[nCount].szEnglish,diction[nCount].szWord);
        ++nCount;
    }
    sort(diction,diction + nCount,CmpDictionitem);
    while(scanf("%s",szWord) == 1) {
        if ((nAnsitem = FindEnglish(szWord,nCount)) != -1) {
            printf("%s\n",diction[nAnsitem].szEnglish);
        } else {
            printf("eh\n");
        }
    }
    return 0;
}
