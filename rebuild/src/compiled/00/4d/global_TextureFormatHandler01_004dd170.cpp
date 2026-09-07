#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_004dd170(unsigned long variant);
struct ListNodeOwner_004dd170 { ListNodeEntry* node; unsigned long count; ListNodeOwner_004dd170* TextureFormat_Handler_01(unsigned long unused,unsigned long variant); };
ListNodeOwner_004dd170* ListNodeOwner_004dd170::TextureFormat_Handler_01(unsigned long,unsigned long variant){ListNode_AllocateNode_004dd170(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}