#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall Stack_Initialize(unsigned long variant);
struct ListNodeOwner_004301c9 { ListNodeEntry* node; unsigned long count; ListNodeOwner_004301c9* ListNode_Construct(unsigned long unused, unsigned long variant); };
ListNodeOwner_004301c9* ListNodeOwner_004301c9::ListNode_Construct(unsigned long, unsigned long variant) { Stack_Initialize(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }