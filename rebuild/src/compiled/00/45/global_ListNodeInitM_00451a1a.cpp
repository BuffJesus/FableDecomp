#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_Variant_M(unsigned long variant);
struct ListNodeOwner_00451a1a { ListNodeEntry* node; unsigned long count; ListNodeOwner_00451a1a* ListNode_Init_M(unsigned long unused, unsigned long variant); };
ListNodeOwner_00451a1a* ListNodeOwner_00451a1a::ListNode_Init_M(unsigned long, unsigned long variant) { ListNode_AllocateNode_Variant_M(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }