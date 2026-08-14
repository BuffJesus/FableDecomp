#pragma optimize("s",on)
// Byte-exact reconstruction of vector<CIntelligentPointer<CThing const*>>::_Destroy
// retail 0x004da636
//
// Element is 0x44 bytes; vtable at offset 0, virtual slot 0 called with (this, 0).

struct Elem {
    virtual void vslot0(int);   // slot 0
    char pad[0x44 - 4];
};

// __fastcall member _Destroy(first, last) -> ecx=this(first via allocator wrapper),
// but retail uses ecx=first, edx=last as the two iterators.
// Model as __fastcall: ecx = first, edx = last.
void __fastcall _Destroy(Elem* first, Elem* last, void* unused)
{
    (void)unused;   // third stack param -> ret 4
    for (; first != last; first = (Elem*)((char*)first + 0x44))
    {
        first->vslot0(0);
    }
}