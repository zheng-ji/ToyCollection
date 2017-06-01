#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int num;
    struct Node * next;
    struct Node * pre;
} Node;

typedef struct dlist {
    Node * head;
    Node * tail;
} dlist;

static Node* initNode(int num) {
    Node * node = NULL;
    node = (Node *) malloc(sizeof(Node));
    if (node == NULL) return NULL;
    node -> num = num;
    node -> next = NULL;
    node -> pre = NULL;
    return node;
}

static dlist* initDlist() {
    dlist * list = NULL;
    list = (dlist *) malloc(sizeof(dlist));
    if (list == NULL) return NULL;
    list -> head = NULL;
    list -> tail = NULL;
    return list;
}

static int numOfDlist(dlist * list) {
    int num = 0;
    Node * head = list -> head;
    while (head) {
        head = head -> next;
        num ++;
    }
    return num;
}

static void displayDlist(dlist *list) {
    Node * head = list->head;
    while (head) {
        printf("Element:%d\n", head->num);
        head = head -> next;
    }
    return;
}

static void freeDllist(dlist *list) {
    Node * head = list -> head;
    Node * now = NULL;
    while (head -> next) {
        now = head -> next;
        head = head -> next;
        if (now) {
            free(now);
            now = NULL;
        }
    }
    return;
}

static int insertDlist(dlist **list, int num) {
    Node * head = (*list) -> head;
    Node * tail = (*list) -> tail;

    if (list == NULL) 
        return -1;

    Node * node = initNode(num);
    if (node == NULL) 
        return -1;

    // empty dlist
    if ((*list) -> head == NULL && (*list) -> tail == NULL) {
        (*list)-> head = node;
        (*list)-> tail = node;
        return 0;
    }

    while (head -> next && head -> num < num) {
        head = head -> next;
    }

    // at the end
    if (head->next == NULL) {
        head -> next = node;
        node -> pre = head;
        tail -> pre = node;
        return 0;
    } else {
        // in the middle
        node-> next = head -> next;
        head -> next -> pre = node;
        node -> pre = head;
        head -> next = node;
        return 0;
    }
}

static int deleteDlist(dlist ** list, int location) {
    Node * head = (*list) -> head;
    Node * now = NULL;
    Node * last = NULL;

    if (head == NULL)
        return -1;
    if (location <= 0 || location > numOfDlist(*list))
        return -1;

    if (location == 1) {
        now = head;
        (*list) -> head = now ->next;
        head -> next ->pre = NULL;
        if (now) {
            free(now);
            now = NULL;
        }
        return 0;
    }
    int num = 0;
    while (head && num++ < location) {
        head = head -> next;
    }

    if (head -> next == NULL) {
        now = (*list) -> tail;
        head -> pre -> next = NULL;
        now -> pre = head->pre;
        if (head) {
            free(head);
            head = NULL;
        }
    } else {
        now = head -> next;
        last = head -> pre; 
        now ->pre = head -> pre;
        last -> next = head ->next;
        if (head) {
            free(head);
            head = NULL;
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    dlist* list = initDlist();
    if (list == NULL)
        return -1;
    insertDlist(&list, 3);
    insertDlist(&list, 5);
    insertDlist(&list, 4);
    printf("num of dlist:%d\n", numOfDlist(list));
    displayDlist(list);
    deleteDlist(&list, 1);
    printf("num of dlist:%d\n", numOfDlist(list));
    displayDlist(list);
    freeDllist(list);
    return 0;
}
