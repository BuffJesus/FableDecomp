#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_0057962d(unsigned long variant);
struct ListNodeOwner_0057962d { ListNodeEntry* node; unsigned long count; ListNodeOwner_0057962d* Container_InitMedium1(unsigned long unused,unsigned long variant); };
ListNodeOwner_0057962d* ListNodeOwner_0057962d::Container_InitMedium1(unsigned long,unsigned long variant){ListNode_AllocateNode_0057962d(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}