#include <stdio.h>

struct Node { Node* next; Node* prev; unsigned long value; };

static Node** Insert(Node** out, Node* node, Node* inserted, unsigned long value)
{
    inserted->value = value;
    Node* previous = node->prev;
    inserted->next = node;
    inserted->prev = previous;
    previous->next = inserted;
    node->prev = inserted;
    *out = inserted;
    return out;
}

int main()
{
    Node left;
    Node right;
    Node inserted;
    Node* result = 0;
    left.next = &right;
    left.prev = &right;
    right.next = &left;
    right.prev = &left;
    if (Insert(&result, &right, &inserted, 0x55aaUL) != &result) return 1;
    if (result != &inserted || left.next != &inserted || inserted.prev != &left) return 2;
    if (inserted.next != &right || right.prev != &inserted || inserted.value != 0x55aaUL) return 3;
    puts("LIST_INSERT_PAIR_PASS");
    return 0;
}