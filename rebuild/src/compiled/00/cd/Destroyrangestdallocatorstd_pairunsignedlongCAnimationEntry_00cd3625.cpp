#pragma optimize("s",on)
// Reconstruction of std::_Destroy_range<allocator<pair<unsigned long, CAnimationEntry>>>
// retail 0x00cd3625, __fastcall(first /*ecx*/, last /*edx*/, allocator* /*stack, ret 4*/)

struct CAnimationEntry {
    // 12 bytes of members so the pair is 16 bytes total (key u32 + 12)
    unsigned long a;
    unsigned long b;
    unsigned long c;
    // destructor is the call target at 0x4aa840
    ~CAnimationEntry();
};

struct AnimPair {
    unsigned long first;
    CAnimationEntry second;
};

struct AnimAlloc {
    char dummy;
};

// The element destroy helper (call [0x4aa840]); modeled as a __fastcall taking this in ecx.
void __fastcall DestroyPair(AnimPair* self);

void __fastcall Destroy_range(AnimPair* first, AnimPair* last, AnimAlloc* al)
{
    (void)al;
    for (; first != last; ++first)
        DestroyPair(first);
}