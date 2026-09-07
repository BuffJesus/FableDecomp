#pragma optimize("s",on)
struct NodeInitEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; NodeInitEntry* next; NodeInitEntry* prev; };
void __stdcall Allocator_Alloc_56bytes(unsigned long variant);
struct NodeInitOwner_0042b72b { NodeInitEntry* node; unsigned long count; NodeInitOwner_0042b72b* CTexture_InitEmpty_56bytes(unsigned long unused, unsigned long variant); };
NodeInitOwner_0042b72b* NodeInitOwner_0042b72b::CTexture_InitEmpty_56bytes(unsigned long, unsigned long variant) { Allocator_Alloc_56bytes(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }