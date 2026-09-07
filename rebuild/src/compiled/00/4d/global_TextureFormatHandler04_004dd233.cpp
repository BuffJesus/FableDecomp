#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_004dd233(unsigned long variant);
struct ListNodeOwner_004dd233 { ListNodeEntry* node; unsigned long count; ListNodeOwner_004dd233* TextureFormat_Handler_04(unsigned long unused,unsigned long variant); };
ListNodeOwner_004dd233* ListNodeOwner_004dd233::TextureFormat_Handler_04(unsigned long,unsigned long variant){ListNode_AllocateNode_004dd233(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}