#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_Variant_N(unsigned long variant);
struct ListNodeOwner_00451a5b { ListNodeEntry* node; unsigned long count; ListNodeOwner_00451a5b* ListNode_Init_N(unsigned long unused, unsigned long variant); };
ListNodeOwner_00451a5b* ListNodeOwner_00451a5b::ListNode_Init_N(unsigned long, unsigned long variant) { ListNode_AllocateNode_Variant_N(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }