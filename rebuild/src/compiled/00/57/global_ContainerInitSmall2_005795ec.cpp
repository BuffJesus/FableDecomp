#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_005795ec(unsigned long variant);
struct ListNodeOwner_005795ec { ListNodeEntry* node; unsigned long count; ListNodeOwner_005795ec* Container_InitSmall2(unsigned long unused,unsigned long variant); };
ListNodeOwner_005795ec* ListNodeOwner_005795ec::Container_InitSmall2(unsigned long,unsigned long variant){ListNode_AllocateNode_005795ec(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}