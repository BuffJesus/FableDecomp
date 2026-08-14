#pragma optimize("s",on)
// Byte-exact reconstruction of _Destroy_range<allocator<pair<unsigned long,CAnimationEntry>>>
// retail 0x006556d3

struct AllocT { char dummy; };

struct Pair {
    unsigned long first;
    void* second;   // CAnimationEntry (4 bytes) -> pair size 8
    // element ptr = this (ecx), allocator = one stack arg
    void destroy_with(AllocT* al);   // implicit thiscall (this in ecx)
};

// _Destroy_range(first=ecx, last=edx, al=stack)
void __fastcall Destroy_range(Pair* first, Pair* last, AllocT* al)
{
    for (; first != last; ++first)
        first->destroy_with(al);
}