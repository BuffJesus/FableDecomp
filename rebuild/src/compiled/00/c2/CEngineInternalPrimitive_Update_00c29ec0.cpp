#include "rebuild_abi.h"
// CEngineInternalPrimitive::Update @ 0x00c29ec0
// mov eax,[esp+4]; mov [eax+0x30],ecx; mov edx,[ecx+8]; mov [eax+0x34],edx; mov [ecx+8],eax; ret 4
// Push-front into an intrusive list held at this->f8: node->owner=this, node->next=this->head, this->head=node.
struct CEngineInternalPrimitive;
struct CListNode { char pad00[0x30]; CEngineInternalPrimitive *owner; CListNode *next; };
struct CEngineInternalPrimitive { char pad00[8]; CListNode *head; void Update(CListNode *node); };
void CEngineInternalPrimitive::Update(CListNode *node)
{
    node->owner = this;
    node->next = head;
    head = node;
}