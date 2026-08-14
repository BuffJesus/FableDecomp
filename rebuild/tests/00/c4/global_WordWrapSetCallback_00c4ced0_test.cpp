#include <stdio.h>

struct Node {
    void* cb;
    Node* prev;
    Node* next;
    void WordWrap_SetCallback(Node* a0, Node* a1);
};

void Node::WordWrap_SetCallback(Node* a0, Node* a1)
{
    if (a0)
        a0->next = this;
    if (a1)
        a1->prev = this;
    this->prev = a0;
    this->next = a1;
}

int main()
{
    Node a, b, cur;
    a.cb = b.cb = cur.cb = 0;
    a.prev = a.next = 0;
    b.prev = b.next = 0;
    cur.prev = cur.next = 0;

    cur.WordWrap_SetCallback(&a, &b);

    int ok = 1;
    if (a.next != &cur) ok = 0;
    if (b.prev != &cur) ok = 0;
    if (cur.prev != &a) ok = 0;
    if (cur.next != &b) ok = 0;

    // null-arg branches
    Node c;
    c.cb = 0; c.prev = c.next = (Node*)0x1234;
    c.WordWrap_SetCallback(0, 0);
    if (c.prev != 0) ok = 0;
    if (c.next != 0) ok = 0;

    if (ok) printf("WORDWRAP_LINK_OK\n");
    else printf("FAIL\n");
    return 0;
}