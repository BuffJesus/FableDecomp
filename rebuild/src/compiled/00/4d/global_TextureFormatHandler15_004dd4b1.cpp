#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_004dd4b1(unsigned long variant);
struct ListNodeOwner_004dd4b1 { ListNodeEntry* node; unsigned long count; ListNodeOwner_004dd4b1* TextureFormat_Handler_15(unsigned long unused,unsigned long variant); };
ListNodeOwner_004dd4b1* ListNodeOwner_004dd4b1::TextureFormat_Handler_15(unsigned long,unsigned long variant){ListNode_AllocateNode_004dd4b1(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}