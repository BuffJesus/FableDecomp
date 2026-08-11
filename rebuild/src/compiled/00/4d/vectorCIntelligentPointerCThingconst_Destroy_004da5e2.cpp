#pragma optimize("s",on)
// std::vector<CIntelligentPointer<CThing const> >::_Destroy
// Iterates [first,last) destroying each 0x30-byte element via virtual slot 0.

struct Elem {
    virtual void dtor_slot0(int flag);
    // element is 0x30 bytes; padding to stride
    char pad[0x30 - sizeof(void*)];
};

// ecx = first, edx = last, [stack] = allocator/this (unused, cleaned by ret 4)
void __fastcall vec_Destroy(Elem* first, Elem* last, void* alloc)
{
    (void)alloc;
    for (; first != last; ++first)
        first->dtor_slot0(0);
}