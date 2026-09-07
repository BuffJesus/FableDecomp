#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_Variant_B(unsigned long variant);
struct ListNodeOwner_00451739 { ListNodeEntry* node; unsigned long count; ListNodeOwner_00451739* ListNode_Init_B(unsigned long unused, unsigned long variant); };
ListNodeOwner_00451739* ListNodeOwner_00451739::ListNode_Init_B(unsigned long, unsigned long variant) { ListNode_AllocateNode_Variant_B(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }