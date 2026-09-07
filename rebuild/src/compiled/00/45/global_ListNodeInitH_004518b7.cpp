#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_Variant_H(unsigned long variant);
struct ListNodeOwner_004518b7 { ListNodeEntry* node; unsigned long count; ListNodeOwner_004518b7* ListNode_Init_H(unsigned long unused, unsigned long variant); };
ListNodeOwner_004518b7* ListNodeOwner_004518b7::ListNode_Init_H(unsigned long, unsigned long variant) { ListNode_AllocateNode_Variant_H(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }