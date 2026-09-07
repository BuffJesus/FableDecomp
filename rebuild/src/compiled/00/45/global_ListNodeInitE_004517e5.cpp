#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_Variant_E(unsigned long variant);
struct ListNodeOwner_004517e5 { ListNodeEntry* node; unsigned long count; ListNodeOwner_004517e5* ListNode_Init_E(unsigned long unused, unsigned long variant); };
ListNodeOwner_004517e5* ListNodeOwner_004517e5::ListNode_Init_E(unsigned long, unsigned long variant) { ListNode_AllocateNode_Variant_E(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }