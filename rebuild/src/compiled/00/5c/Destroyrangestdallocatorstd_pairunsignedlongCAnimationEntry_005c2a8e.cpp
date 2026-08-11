#pragma optimize("s",on)
// _Destroy_range<allocator<pair<unsigned long, CAnimationEntry>>>
// retail 0x005c2a8e: loop over [first,last) stride 0x2c calling ~pair() (thiscall),
// pop edi/esi, ret 4. The element (std::pair<unsigned long, CAnimationEntry>) is 44
// bytes (0x2c) and its destructor is the out-of-line thiscall at 0x5c2739.

struct AnimPair {
    unsigned long first;          // key
    unsigned long second[10];     // CAnimationEntry payload -> total 44 (0x2c)
    void Dtor();                  // stands in for ~pair(); defined out-of-line
};

struct AllocPair {};

void __fastcall _Destroy_range(AnimPair* first, AnimPair* last, AllocPair* al)
{
    for (; first != last; ++first)
        first->Dtor();
}