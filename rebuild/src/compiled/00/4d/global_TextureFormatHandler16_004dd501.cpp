#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_004dd501(unsigned long variant);
struct ListNodeOwner_004dd501 { ListNodeEntry* node; unsigned long count; ListNodeOwner_004dd501* TextureFormat_Handler_16(unsigned long unused,unsigned long variant); };
ListNodeOwner_004dd501* ListNodeOwner_004dd501::TextureFormat_Handler_16(unsigned long,unsigned long variant){ListNode_AllocateNode_004dd501(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}