// Byte-exact reconstruction of _Destroy_range<allocator<pair<unsigned_long,CAnimationEntry>>>
// retail 0x0065572d (31 bytes). __fastcall(first /*ecx*/, last /*edx*/, allocator /*stack*/).
// Element size 8. The element-destroy helper (retail 0x4a9760) is __fastcall: this=pair* in ecx,
// allocator pushed on the stack -> modelled as a real member taking the allocator by value.
// Retail was emitted favoring size (no register caching of the allocator slot; it re-pushes
// [esp+0xc] each iteration and enters the loop via jmp-to-condition), reproduced with
// #pragma optimize("s") under the harness's /O2 /Oy.

struct CAnimationEntry { unsigned long a; };
struct Alloc { unsigned long d; }; // std::allocator<...> — empty, passed as a single dword

struct Pair {
    unsigned long first;
    CAnimationEntry second; // total 8 bytes
    void destroy(Alloc al); // __fastcall element destroy (retail 0x4a9760); this=ecx, al on stack
};

#pragma optimize("s", on)
void __fastcall Destroy_range_std_allocator_std_pair_unsigned_long_CAnimationEntry_____(Pair* first, Pair* last, Alloc al)
{
    for (; first != last; ++first)
        first->destroy(al);
}