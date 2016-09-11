/*
 * 第一个节点不动，从第二个节点以后，每个节点挪到第一个节点的后面，
 * 最后的节点与第一个节点组成环，然后断掉原本head,使得head的下一节点是NULL
 */
node *reverse_linklist(node * head) {
    node *p, *q;
    p = head -> next;
    while (p->next != NULL) {
        q = p;
        p ->next = q->next;
        q->next = head ->next;
        head ->next = q;
    }
    p->next = head;
    head = head ->next;
    p->next->next = NULL;
    return head;
}

/*
 * 维护pre, cur, next;
 * next 是 为了不丢失 current->next 指针
 */
node *reverse_linklist(node *head) {
    node * current = head;
    node * pre = NULL;
    node * next = NULL;
    for (; current != NULL; current = next) {
        next = current ->next;
        current ->next = pre;
        pre = current;
    }
    return pre;
}
