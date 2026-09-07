#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_005b21ca(unsigned long variant);
struct ListNodeOwner_005b21ca { ListNodeEntry* node; unsigned long count; ListNodeOwner_005b21ca* Render_InitializeNode_24Byte_Var2(unsigned long unused,unsigned long variant); };
ListNodeOwner_005b21ca* ListNodeOwner_005b21ca::Render_InitializeNode_24Byte_Var2(unsigned long,unsigned long variant){ListNode_AllocateNode_005b21ca(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}