#include <stdlib.h>
#include <stdio.h>

struct LinkedListNodeOverlay
{
    LinkedListNodeOverlay* prev;
    LinkedListNodeOverlay* next;
};

static void Unlink(LinkedListNodeOverlay** list_ptr, LinkedListNodeOverlay* node)
{
    LinkedListNodeOverlay* next = node->next;
    LinkedListNodeOverlay* prev = node->prev;
    next->prev = prev;
    prev->next = next;
    free(node);
    *list_ptr = prev;
}

int main()
{
    LinkedListNodeOverlay left;
    LinkedListNodeOverlay right;
    LinkedListNodeOverlay* node = (LinkedListNodeOverlay*)malloc(sizeof(LinkedListNodeOverlay));
    LinkedListNodeOverlay* cursor = node;
    left.prev = &right;
    left.next = node;
    right.prev = node;
    right.next = &left;
    node->prev = &left;
    node->next = &right;
    Unlink(&cursor, node);
    if (left.next != &right || right.prev != &left || cursor != &left) return 1;
    puts("LINKEDLIST_REMOVE_NODE_PASS");
    return 0;
}