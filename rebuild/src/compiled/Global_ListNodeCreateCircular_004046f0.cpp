#include <stdlib.h>

struct CircularListNode
{
    CircularListNode* next;
    CircularListNode* previous;
    unsigned char reserved[8];
};

void** __fastcall ListNode_Create_Circular(void** nodeRef, void*, void*)
{
    *nodeRef = 0;
    CircularListNode* node = static_cast<CircularListNode*>(malloc(0x10));
    node->next = node;
    node->previous = node;
    *nodeRef = node;
    return nodeRef;
}
