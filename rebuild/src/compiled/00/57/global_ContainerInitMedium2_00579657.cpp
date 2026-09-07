#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall ListNode_AllocateNode_00579657(unsigned long variant);
struct ListNodeOwner_00579657 { ListNodeEntry* node; unsigned long count; ListNodeOwner_00579657* Container_InitMedium2(unsigned long unused,unsigned long variant); };
ListNodeOwner_00579657* ListNodeOwner_00579657::Container_InitMedium2(unsigned long,unsigned long variant){ListNode_AllocateNode_00579657(variant);count=0;node->state=0;node->payload=0;node->next=node;node->prev=node;return this;}