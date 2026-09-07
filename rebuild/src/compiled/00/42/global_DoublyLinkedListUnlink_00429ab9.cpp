#pragma optimize("s",on)
#include <stdlib.h>

struct LinkedListNodeOverlay
{
    LinkedListNodeOverlay* prev;
    LinkedListNodeOverlay* next;
};

void __stdcall DoublyLinkedList_Unlink(LinkedListNodeOverlay** list_ptr, LinkedListNodeOverlay* node)
{
    LinkedListNodeOverlay* next = node->next;
    LinkedListNodeOverlay* prev = node->prev;
    next->prev = prev;
    prev->next = next;
    free(node);
    *list_ptr = prev;
}