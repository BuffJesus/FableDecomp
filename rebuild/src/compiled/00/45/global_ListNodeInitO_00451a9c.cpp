#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_Variant_O(unsigned long variant);
struct ListNodeOwner_00451a9c { ListNodeEntry* node; unsigned long count; ListNodeOwner_00451a9c* ListNode_Init_O(unsigned long unused, unsigned long variant); };
ListNodeOwner_00451a9c* ListNodeOwner_00451a9c::ListNode_Init_O(unsigned long, unsigned long variant) { ListNode_AllocateNode_Variant_O(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }