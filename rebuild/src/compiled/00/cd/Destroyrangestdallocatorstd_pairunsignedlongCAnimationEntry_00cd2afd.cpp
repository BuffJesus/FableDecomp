#pragma optimize("s",on)
// Byte-exact reconstruction of _Destroy_range<allocator<pair<unsigned long,CAnimationEntry> > >
// retail 0x00cd2afd

struct Alloc { char dummy; };

struct Pair {
    unsigned long a;   // +0
    unsigned long b;   // +4
    unsigned long c;   // +8
    unsigned long d;   // +0xc  -> total 0x10 bytes
    // this-call destroy: ecx=this(element ptr), one stack arg (allocator)
    void destroy(Alloc* al);
};

// _Destroy_range(pair* _First (ecx), pair* _Last (edx), allocator* _Al (stack))
void __fastcall Destroy_range(Pair* _First, Pair* _Last, Alloc* _Al)
{
    for (; _First != _Last; ++_First)
        _First->destroy(_Al);
}