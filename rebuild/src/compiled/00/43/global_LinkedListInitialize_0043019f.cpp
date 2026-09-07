#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall Queue_Initialize(unsigned long variant);
struct ListNodeOwner_0043019f { ListNodeEntry* node; unsigned long count; ListNodeOwner_0043019f* LinkedList_Initialize(unsigned long unused, unsigned long variant); };
ListNodeOwner_0043019f* ListNodeOwner_0043019f::LinkedList_Initialize(unsigned long, unsigned long variant) { Queue_Initialize(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }