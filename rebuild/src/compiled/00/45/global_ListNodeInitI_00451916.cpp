#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_Variant_I(unsigned long variant);
struct ListNodeOwner_00451916 { ListNodeEntry* node; unsigned long count; ListNodeOwner_00451916* ListNode_Init_I(unsigned long unused, unsigned long variant); };
ListNodeOwner_00451916* ListNodeOwner_00451916::ListNode_Init_I(unsigned long, unsigned long variant) { ListNode_AllocateNode_Variant_I(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }