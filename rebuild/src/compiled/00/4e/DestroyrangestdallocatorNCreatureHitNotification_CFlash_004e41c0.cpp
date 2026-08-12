#pragma optimize("s",on)
// Reconstruction of STL _Destroy_range for NCreatureHitNotification::CFlash
// retail 0x004e41c0, __fastcall(first, last, allocator*, _Nonscalar_ptr_iterator_tag)
// Loop calls CFlash scalar-deleting destructor (0x4e1cfc) with flag 0, stride 0x10.

struct CFlash {
    // 0x10 bytes total
    void *vfptr;   // +0
    int   a;       // +4
    int   b;       // +8
    int   c;       // +0xC
    // scalar deleting destructor: __fastcall, one int flag arg
    void Destroy(int flag);
};

template<class T> struct allocator { };

void __fastcall _Destroy_range(
        CFlash *first,
        CFlash *last,
        allocator<CFlash> *al)
{
    while (first != last) {
        first->Destroy(0);
        ++first;
    }
}