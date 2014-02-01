#include <stdio.h>
#include <stdlib.h>

#define MAX_LEVEL 10
typedef struct node {
    int value;
    int key;
    struct node *next[1];
} node;

typedef struct skiplist {
    int level;
    node *header;
} skiplist;

node *createNode(int level, int key, int value) {
    node *ns = (node *) malloc(sizeof(node) + level * sizeof(node *));
    ns->key = key;
    ns->value = value;
    return ns;
}

skiplist * createSkiplist() {
    skiplist * sl = (skiplist*) malloc(sizeof(skiplist));
    sl->level = 0;
    sl->header = createNode(MAX_LEVEL-1, 0, 0);
    int i;
    for (i = 0; i < MAX_LEVEL; i++) {
        sl->header->next[i] = NULL;
    }
    return sl;
}

int randomLevel() {
    int k = 1;
    while (rand() % 2) 
        k++;
    k = (k < MAX_LEVEL) ? k: MAX_LEVEL;
    return k;
}

bool insert(skiplist *sl, int key, int value) {
    node *update[MAX_LEVEL];
    node *p, *q = NULL;
    p = sl->header;
    int k = sl->level;
    int i;
    for (i = k - 1; i >= 0; i--) {
        while ((q = p->next[i]) && (q->key < key)) {
            p = q;
        }
        update[i] = p;
    }
    if (q && q->key == key) {
        return false;
    }
    k = randomLevel();
    if (k > sl->level) {
        for (int i = sl->level; i< k; i++) {
            update[i] = sl->header;
        }
        sl->level = k;
    }
    q = createNode(k, key, value);
    for (int i = 0 ; i < k; i++) {
        q->next[i] = update[i]->next[i];
        update[i] -> next[i] = q;
    }
    return true;
}

int search(skiplist * sl, int key) {
    node * p, *q = NULL;
    p = sl->header;
    int k = sl->level;
    for (int i = k-1; i >= 0; i--) {
        while ((q=p->next[i]) && q->key <= key) {
            if (q->key == key) {
                return q->value;
            }
            p = q;
        }
    }
    return NULL;
}

bool deleteSL(skiplist * sl, int key) {
    node *update[MAX_LEVEL];
    node *p, *q = NULL;
    p = sl ->header;
    int k = sl->level;
    for (int i = k-1; i >= 0; i--) {
        while ((q=p->next[i]) && (q->key < key)) {
            p=q;
        }
        update[i] = p;
    }

    //删除最大的，要更新节点
    if (q&&q->key==key) {
        for (int i = 0; i < sl->level; i++) {
            if (update[i]->next[i]==q) {
                update[i]->next[i] == q->next[i];
            }
        }
        free(q);
        for (int i = sl->level;i >= 0; i--) {
            if (sl->header->next[i] == NULL) {
                sl->level --;
            }
        }
        return true;
    } else {
        return false;
    }
}

void printSL(skiplist *sl) {
    node *p, *q = NULL;
    int k = sl->level;
    for (int i = k-1;i >= 0; i--) {
        p = sl->header;
        while (q = p->next[i]) {
            printf("%d->", p->value);
            p = q;
        }
        printf("\n");
    }
}

int main () {
    skiplist *sl = createSkiplist();
    for (int i=1; i <= 19; i++) {
        insert(sl, i, i*2);
    }
    printSL(sl);
    int i = search(sl, 4);
    printf("i=%d", i);
    bool b = deleteSL(sl, 4);
    if (b) {
        printf("ok\n");
    }
    printSL(sl);
    return 0;
}





