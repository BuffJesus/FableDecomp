#pragma optimize("s",on)
struct ListNodeEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; ListNodeEntry* next; ListNodeEntry* prev; };
void __stdcall CList_AllocateNode(unsigned long variant);
struct ListNodeOwner_00451add { ListNodeEntry* node; unsigned long count; ListNodeOwner_00451add* CList_Initialize(unsigned long unused, unsigned long variant); };
ListNodeOwner_00451add* ListNodeOwner_00451add::CList_Initialize(unsigned long, unsigned long variant) { CList_AllocateNode(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }