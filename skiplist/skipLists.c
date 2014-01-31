#include <stdio.h>
#include <stdlib.h>

#define MAX_LEVEL 10
typedef struct nodeStructure {
    int value;
    int key;
    struct nodeStructure *forward[1];
} nodeStructure;

typedef struct skiplist {
    int level;
    nodeStructure *header;
} skiplist;

nodeStructure *createNode(int level, int key, int value) {
    nodeStructure *ns = (nodeStructure *) malloc(sizeof(nodeStructure) + level * sizeof(nodeStructure *));
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
        sl->header->forward[i] = NULL;
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
    nodeStructure *update[MAX_LEVEL];
    nodeStructure *p, *q = NULL;
    p = sl->header;
    int k = sl->level;
    int i;
    for (i = k - 1; i >= 0; i--) {
        while ((q = p->forward[i]) && (q->key < key)) {
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
        q->forward[i] = update[i]->forward[i];
        update[i] -> forward[i] = q;
    }
    return true;
}

int search(skiplist * sl, int key) {
    nodeStructure * p, *q = NULL;
    p = sl->header;
    int k = sl->level;
    for (int i = k-1; i >= 0; i--) {
        while ((q=p->forward[i]) && q->key <= key) {
            if (q->key == key) {
                return q->value;
            }
            p = q;
        }
    }
    return NULL;
}

bool deleteSL(skiplist * sl, int key) {
    nodeStructure *update[MAX_LEVEL];
    nodeStructure *p, *q = NULL;
    p = sl ->header;
    int k = sl->level;
    for (int i = k-1; i >= 0; i--) {
        while ((q=p->forward[i]) && (q->key < key)) {
            p=q;
        }
        update[i] = p;
    }

    //删除最大的，要更新节点
    if (q&&q->key==key) {
        for (int i = 0; i < sl->level; i++) {
            if (update[i]->forward[i]==q) {
                update[i]->forward[i] == q->forward[i];
            }
        }
        free(q);
        for (int i = sl->level;i >= 0; i--) {
            if (sl->header->forward[i] == NULL) {
                sl->level --;
            }
        }
        return true;
    } else {
        return false;
    }
}

void printSL(skiplist *sl) {
    nodeStructure *p, *q = NULL;
    int k = sl->level;
    for (int i = k-1;i >= 0; i--) {
        p = sl->header;
        while (q = p->forward[i]) {
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





