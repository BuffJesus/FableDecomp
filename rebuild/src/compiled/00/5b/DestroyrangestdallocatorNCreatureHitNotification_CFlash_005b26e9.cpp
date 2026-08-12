#pragma optimize("s",on)
// _Destroy_range<std::allocator<NCreatureHitNotification::CFlash>>
// Retail 0x005b26e9: __fastcall(first, last, alloc*, tag)
// Loops [first,last) stride 4 calling first->Dtor(0) via __fastcall.

struct CFlash {
    void *vptr; // makes sizeof==4 so ++first strides by 4 (retail 'add esi,4')
    // one int arg (the retail 'push 0'): scalar-deleting-destructor style.
    void Dtor(int flags);
};

struct Alloc {};

void __fastcall _Destroy_range_CFlash(CFlash *first, CFlash *last, Alloc *al)
{
    for (; first != last; ++first)
        first->Dtor(0);
}