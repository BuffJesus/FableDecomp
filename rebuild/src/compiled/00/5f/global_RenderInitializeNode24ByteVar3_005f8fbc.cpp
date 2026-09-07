#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_005f8fbc(unsigned long variant);
struct ListNodeOwner_005f8fbc { ListNodeEntry* node; unsigned long count; ListNodeOwner_005f8fbc* Render_InitializeNode_24Byte_Var3(unsigned long unused,unsigned long variant); };
ListNodeOwner_005f8fbc* ListNodeOwner_005f8fbc::Render_InitializeNode_24Byte_Var3(unsigned long,unsigned long variant){ListNode_AllocateNode_005f8fbc(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}