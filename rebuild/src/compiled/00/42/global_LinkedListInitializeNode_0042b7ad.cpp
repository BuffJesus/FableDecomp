#pragma optimize("s",on)
struct NodeInitEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; NodeInitEntry* next; NodeInitEntry* prev; };
void __stdcall LinkedList_Initialize(unsigned long variant);
struct NodeInitOwner_0042b7ad { NodeInitEntry* node; unsigned long count; NodeInitOwner_0042b7ad* LinkedList_InitializeNode(unsigned long unused, unsigned long variant); };
NodeInitOwner_0042b7ad* NodeInitOwner_0042b7ad::LinkedList_InitializeNode(unsigned long, unsigned long variant) { LinkedList_Initialize(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }