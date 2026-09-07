#pragma optimize("s",on)
#include <stdlib.h>

struct DoublyLinkedListSentinel
{
    DoublyLinkedListSentinel* next;
    DoublyLinkedListSentinel* prev;
    unsigned long reserved;
};

void** __fastcall DoublyLinkedList_InitializeEmpty(void** storage, void*, void*)
{
    *storage = 0;
    DoublyLinkedListSentinel* sentinel = (DoublyLinkedListSentinel*)malloc(0x0c);
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    *storage = sentinel;
    return storage;
}