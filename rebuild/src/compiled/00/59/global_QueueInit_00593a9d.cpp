#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_00593a9d(unsigned long variant);
struct ListNodeOwner_00593a9d { ListNodeEntry* node; unsigned long count; ListNodeOwner_00593a9d* Queue_Init(unsigned long unused,unsigned long variant); };
ListNodeOwner_00593a9d* ListNodeOwner_00593a9d::Queue_Init(unsigned long,unsigned long variant){ListNode_AllocateNode_00593a9d(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}