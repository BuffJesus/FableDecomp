#pragma optimize("s",on)
struct NodeInitEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; NodeInitEntry* next; NodeInitEntry* prev; };
void __stdcall Allocator_Alloc_28bytes(unsigned long variant);
struct NodeInitOwner_0042b7ee { NodeInitEntry* node; unsigned long count; NodeInitOwner_0042b7ee* CTexture_InitEmpty_28bytes(unsigned long unused, unsigned long variant); };
NodeInitOwner_0042b7ee* NodeInitOwner_0042b7ee::CTexture_InitEmpty_28bytes(unsigned long, unsigned long variant) { Allocator_Alloc_28bytes(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }