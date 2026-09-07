#pragma optimize("s",on)
#include <cstdlib>
struct LinkedListNode
{
    LinkedListNode* next;
    LinkedListNode* previous;
    unsigned char reserved[0x18];
};
void** __fastcall LinkedList_Initialize(
    void** listHead,
    void* unusedRegisterArgument,
    void* unusedStackArgument)
{
    (void)unusedRegisterArgument;
    (void)unusedStackArgument;
    *(unsigned long*)listHead &= 0;
    LinkedListNode* node = (LinkedListNode*)std::malloc(0x20);
    node->next = node;
    node->previous = node;
    *listHead = node;
    return listHead;
}