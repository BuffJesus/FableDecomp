#pragma optimize("s",on)
// _Destroy_range<std::allocator<std::pair<unsigned_long,CAnimationEntry>>>
// retail 004dbe73, __fastcall(first, last, allocator&)
// loop over [first,last) stride 0xc calling element dtor via thiscall.

struct CAnimEntryPair { unsigned long k; unsigned long a; unsigned long b; }; // 12 bytes
extern "C" void __fastcall Pair_Dtor(CAnimEntryPair* self);   // external dtor @0xa01c10 (thiscall)

struct AllocDummy {};

void __fastcall _Destroy_range_pair(CAnimEntryPair* first, CAnimEntryPair* last, AllocDummy* al)
{
    (void)al;
    while (first != last)
    {
        Pair_Dtor(first);
        ++first;
    }
}