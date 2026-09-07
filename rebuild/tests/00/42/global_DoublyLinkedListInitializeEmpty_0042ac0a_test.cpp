#include <stdlib.h>
#include <stdio.h>

struct Node { Node* next; Node* prev; unsigned long reserved; };

static void** Initialize(void** storage)
{
    *storage = 0;
    Node* sentinel = (Node*)malloc(12);
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    *storage = sentinel;
    return storage;
}

int main()
{
    void* head = (void*)1;
    if (Initialize(&head) != &head || !head) return 1;
    Node* sentinel = (Node*)head;
    if (sentinel->next != sentinel || sentinel->prev != sentinel) return 2;
    free(sentinel);
    puts("DLIST_INITIALIZE_EMPTY_PASS");
    return 0;
}