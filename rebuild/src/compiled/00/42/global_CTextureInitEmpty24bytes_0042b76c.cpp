#pragma optimize("s",on)
struct NodeInitEntry { unsigned char state; unsigned char padding[3]; unsigned long payload; NodeInitEntry* next; NodeInitEntry* prev; };
void __stdcall Allocator_Alloc_24bytes(unsigned long variant);
struct NodeInitOwner_0042b76c { NodeInitEntry* node; unsigned long count; NodeInitOwner_0042b76c* CTexture_InitEmpty_24bytes(unsigned long unused, unsigned long variant); };
NodeInitOwner_0042b76c* NodeInitOwner_0042b76c::CTexture_InitEmpty_24bytes(unsigned long, unsigned long variant) { Allocator_Alloc_24bytes(variant); count=0; node->state=0; node->payload=0; node->next=node; node->prev=node; return this; }