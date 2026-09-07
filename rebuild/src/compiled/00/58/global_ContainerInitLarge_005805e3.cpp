#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_005805e3(unsigned long variant);
struct ListNodeOwner_005805e3 { ListNodeEntry* node; unsigned long count; ListNodeOwner_005805e3* Container_InitLarge(unsigned long unused,unsigned long variant); };
ListNodeOwner_005805e3* ListNodeOwner_005805e3::Container_InitLarge(unsigned long,unsigned long variant){ListNode_AllocateNode_005805e3(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}