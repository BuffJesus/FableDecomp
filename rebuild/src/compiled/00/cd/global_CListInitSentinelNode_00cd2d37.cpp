#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_00cd2d37(unsigned long variant);
struct ListNodeOwner_00cd2d37 { ListNodeEntry* node; unsigned long count; ListNodeOwner_00cd2d37* CList_InitSentinelNode(unsigned long unused,unsigned long variant); };
ListNodeOwner_00cd2d37* ListNodeOwner_00cd2d37::CList_InitSentinelNode(unsigned long,unsigned long variant){ListNode_AllocateNode_00cd2d37(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}