#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode(unsigned long variant);
struct ListNodeOwner_0045170f { ListNodeEntry* node; unsigned long count; ListNodeOwner_0045170f* ListNode_Init(unsigned long unused, unsigned long variant); };
ListNodeOwner_0045170f* ListNodeOwner_0045170f::ListNode_Init(unsigned long, unsigned long variant) { ListNode_AllocateNode(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }