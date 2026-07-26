#include <stdlib.h>

struct StdListNodeInitNode
{
    StdListNodeInitNode* next;
    StdListNodeInitNode* previous;
    unsigned char reserved[8];
};

void** __fastcall std_list_node_Init(void** nodeRef, void*, void*)
{
    *nodeRef = 0;
    StdListNodeInitNode* node =
        static_cast<StdListNodeInitNode*>(malloc(0x10));
    node->next = node;
    node->previous = node;
    *nodeRef = node;
    return nodeRef;
}
