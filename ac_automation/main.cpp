#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;
struct node{
    int count;
    struct node *fail;
    struct node *next[26];
    node()
    {
        fail=NULL;
        memset(next,NULL,sizeof(next));
        count=0;
    }
}*queue[500005];
struct node *root;
char keyword[55],str[1000005];
void build(char *str)//建立字典树
{
    struct node *p;
    p=root;
    int index;
    for(;*str!='\0';str++)
    {
        index=*str-'a';
        if(p->next[index]==NULL)
            p->next[index]=new node;
        p=p->next[index];
    }
    p->count++;
}
void AC_tree()//构造失败指针
{
    int f,r,i;
    struct node *p,*temp;
    root->fail=NULL;
    f=0;r=0;
    queue[r++]=root;
    while(f!=r)
    {
        p=queue[f++];
        for(i=0;i<26;i++)
        {
            if(p->next[i]!=NULL)
            {
                temp=p->fail;
                while(temp!=NULL)
                {
                    if(temp->next[i]!=NULL)
                    {
                        p->next[i]->fail=temp->next[i];
                        break;
                    }
                    temp=temp->fail;
                }
                if(temp==NULL)
                    p->next[i]->fail=root;
                queue[r++]=p->next[i];
            }
        }
    }
}
int find(char *str)//单词查找
{
    int index;
    int sum=0;
    struct node *p,*temp;
    p=root;
    for(;*str!='\0';str++)
    {
        index=*str-'a';
        while(p->next[index]==NULL&&p!=root)
            p=p->fail;
        p=p->next[index];
        if(p==NULL)
            p=root;
        temp=p;
        while(temp!=root&&temp->count!=-1)
        {
            sum=sum+temp->count;
            temp->count=-1;
            temp=temp->fail;
        }
    }
    return sum;
}
void del(struct node *root)
{
    int i;
    for(i=0;i<26;i++)
    {
        if(root->next[i]!=NULL)
            del(root->next[i]);
    }
    delete(root);
}
int main()
{
    int T,n;
    while(scanf("%d",&T)!=EOF)
    {
        while(T--)
        {
            root=new node;
            scanf("%d",&n);
            getchar();
            while(n--)
            {
                scanf("%s",keyword);
                build(keyword);
            }
            AC_tree();
            scanf("%s",str);
            printf("%d\n",find(str));
            del(root);
        }
    }
    return 0;
}
