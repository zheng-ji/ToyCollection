#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <utility>
using std::string;
using std::vector;
using std::pair;
using std::make_pair;

#define CHAR_SET_SIZE 26
#define PATTERN_SIZE 300
#define QUERY_SIZE 3000
#define QSIZE 300000

struct node
{
    node *fail;
    node *child[CHAR_SET_SIZE];
    int point;
    node() {
        fail = NULL;
        for (int i = 0; i < CHAR_SET_SIZE; ++i) child[i] = NULL;
        point = -1;
    }
};

node *Q[QSIZE];
node *Root;

vector <string> Pattern_Collection;

void Init()
{
    Root = new node;
}

void Insert(char *s, int num)
{
    node *p = Root;
    for (char *c = s; *c != '\0'; ++c)
    {
        int t = (*c) - 'a';
        if (p->child[t] == NULL)
        {
            p->child[t] = new node;
        }
        p = p->child[t];
        if ((*(c+1)) == '\0') p->point = num; 
    }
}
void InputPattern()
{
    printf("Input number of patterns:");
    fflush(stdout);
    int N;
    scanf("%d",&N);
    char s[PATTERN_SIZE];
    for (int i = 1; i <= N; ++i)
    {
        scanf("%s", s);
        Pattern_Collection.push_back(s);
        Insert(s, Pattern_Collection.size()-1);
    }
}

void BuildFailPoint()
{
    int Qh = 0, Qt = 1;
    Q[1] = Root;
    while (Qh < Qt)
    {
        node *now = Q[++Qh];
        for (int i = 0; i < CHAR_SET_SIZE; ++i)
        {
            if (now->child[i] != NULL)
            {
                if (now == Root) now->child[i]->fail = Root;
                else
                {
                    node *p = now->fail;
                    while (p != NULL)
                    {
                        if (p->child[i] != NULL)
                        {
                            now->child[i]->fail = p->child[i];
                            break;
                        }
                        p = p->fail;
                    }
                    if (p == NULL) now->child[i]->fail = Root;
                }
                Q[++Qt] = now->child[i];
            }
        }
    }
}

char QueryString[QUERY_SIZE];

vector <pair <int,int> > Query()
{
    vector <pair <int,int> > Ret;
    int Len = strlen(QueryString);
    node *p = Root;
    for (int i = 0; i != Len; ++i)
    {
        int index = QueryString[i] - 'a';
        while (p->child[index] == NULL && p != Root) p = p->fail;
        if (p->child[index] == NULL) continue;
        p = p->child[index];
        node *t = p;
        while (t != Root)
        {
            if (t->point != -1) Ret.push_back(make_pair(t->point, i));
            t = t->fail;
        }
    }
    return Ret;
}

void InputQuery()
{
    printf("Input the query string:\n");
    scanf("%s", QueryString);
    vector < pair <int,int> > QueryAns = Query();
    for (int i = 0;i != QueryAns.size(); ++i)
    {
        printf("Found pattern \"%s\" at %d\n",
                Pattern_Collection[QueryAns[i].first].c_str(),
                QueryAns[i].second-Pattern_Collection[QueryAns[i].first].size()+1); 
    }
}

int main()
{
    Init();
    InputPattern();
    BuildFailPoint();
    InputQuery();
    return 0;
}
