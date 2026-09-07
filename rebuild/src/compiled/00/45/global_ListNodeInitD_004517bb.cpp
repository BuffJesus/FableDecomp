#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_Variant_D(unsigned long variant);
struct ListNodeOwner_004517bb { ListNodeEntry* node; unsigned long count; ListNodeOwner_004517bb* ListNode_Init_D(unsigned long unused, unsigned long variant); };
ListNodeOwner_004517bb* ListNodeOwner_004517bb::ListNode_Init_D(unsigned long, unsigned long variant) { ListNode_AllocateNode_Variant_D(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }