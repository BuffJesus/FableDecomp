#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_005b2136(unsigned long variant);
struct ListNodeOwner_005b2136 { ListNodeEntry* node; unsigned long count; ListNodeOwner_005b2136* Render_InitializeNode_24Byte_Var(unsigned long unused,unsigned long variant); };
ListNodeOwner_005b2136* ListNodeOwner_005b2136::Render_InitializeNode_24Byte_Var(unsigned long,unsigned long variant){ListNode_AllocateNode_005b2136(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}