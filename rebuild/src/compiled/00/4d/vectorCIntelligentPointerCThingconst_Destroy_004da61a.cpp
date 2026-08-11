#pragma optimize("s",on)
// vector<CIntelligentPointer<CThing const*>,...>::_Destroy
// Range destroy helper: for each element in [first,last) step sizeof(elem)=0x48,
// call virtual slot 0 with arg 0 (scalar deleting destructor style).

struct CIntelPtr {
    virtual void vfn0(int flag);   // slot 0 at [vtable+0]
    // pad element to 0x48 bytes (72). vtable ptr is 4 bytes; add filler.
    char pad[0x48 - 4];
};

// __fastcall in sig; harness rewrites to __fastcall. Model as free __fastcall:
//  ecx = first, edx = last, plus a trailing stack allocator arg (ret 4).
void __fastcall vector_Destroy(CIntelPtr* first, CIntelPtr* last, void* alloc)
{
    (void)alloc;
    for (; first != last; ++first)
        first->vfn0(0);
}