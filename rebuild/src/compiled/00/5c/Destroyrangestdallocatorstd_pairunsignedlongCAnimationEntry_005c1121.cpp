#pragma optimize("s",on)
// Byte-exact reconstruction of std::_Destroy_range<allocator<pair<unsigned long,CAnimationEntry>>>
// retail 0x005c1121

struct Allocator { char dummy; };

struct Pair {
    unsigned long key;
    char value[0x10];      // total sizeof = 0x14
    void Destroy(Allocator* al);   // __fastcall: ecx=this, stack=al, ret 4
};

void __fastcall Destroy_range_pair_unsigned_long_CAnimationEntry(
        Pair* first, Pair* last, Allocator* al)
{
    for (; first != last; ++first)
        first->Destroy(al);
}