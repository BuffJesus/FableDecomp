#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_004dd405(unsigned long variant);
struct ListNodeOwner_004dd405 { ListNodeEntry* node; unsigned long count; ListNodeOwner_004dd405* TextureFormat_Handler_12(unsigned long unused,unsigned long variant); };
ListNodeOwner_004dd405* ListNodeOwner_004dd405::TextureFormat_Handler_12(unsigned long,unsigned long variant){ListNode_AllocateNode_004dd405(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}