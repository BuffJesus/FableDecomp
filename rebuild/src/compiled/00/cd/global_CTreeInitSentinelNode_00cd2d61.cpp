#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_00cd2d61(unsigned long variant);
struct ListNodeOwner_00cd2d61 { ListNodeEntry* node; unsigned long count; ListNodeOwner_00cd2d61* CTree_InitSentinelNode(unsigned long unused,unsigned long variant); };
ListNodeOwner_00cd2d61* ListNodeOwner_00cd2d61::CTree_InitSentinelNode(unsigned long,unsigned long variant){ListNode_AllocateNode_00cd2d61(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}