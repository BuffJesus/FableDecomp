#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_Variant_C(unsigned long variant);
struct ListNodeOwner_0045177a { ListNodeEntry* node; unsigned long count; ListNodeOwner_0045177a* ListNode_Init_C(unsigned long unused, unsigned long variant); };
ListNodeOwner_0045177a* ListNodeOwner_0045177a::ListNode_Init_C(unsigned long, unsigned long variant) { ListNode_AllocateNode_Variant_C(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }