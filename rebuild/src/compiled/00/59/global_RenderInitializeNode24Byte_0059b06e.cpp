#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_0059b06e(unsigned long variant);
struct ListNodeOwner_0059b06e { ListNodeEntry* node; unsigned long count; ListNodeOwner_0059b06e* Render_InitializeNode_24Byte(unsigned long unused,unsigned long variant); };
ListNodeOwner_0059b06e* ListNodeOwner_0059b06e::Render_InitializeNode_24Byte(unsigned long,unsigned long variant){ListNode_AllocateNode_0059b06e(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}