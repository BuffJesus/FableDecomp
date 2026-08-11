#pragma optimize("s",on)
// vector<CIntelligentPointer<CThing const>>::_Destroy(first, last, alloc)
// STL range-destroy helper. __fastcall: ecx=first iterator, edx=last iterator,
// one stack allocator arg (cleaned by ret 4). For each element in [first,last)
// invoke the scalar-deleting destructor (vtable slot 0) with flag 0. Element
// stride is 0x30 bytes.

struct Elem {
    virtual void dtor(int flag);
    char pad[0x30 - 4];
};

void __fastcall _Destroy(Elem* first, Elem* last, void* alloc)
{
    for (; first != last; ++first)
        first->dtor(0);
    (void)alloc;
}