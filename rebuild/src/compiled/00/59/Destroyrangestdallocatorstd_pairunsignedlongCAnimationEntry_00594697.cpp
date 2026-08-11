#pragma optimize("s",on)
// _Destroy_range<allocator<pair<unsigned long, CAnimationEntry> > >
// Retail 0x00594697. STL destroy-range loop over 8-byte elements.

struct AnimPair {
    unsigned long key;
    unsigned long val;   // CAnimationEntry modeled as a 4-byte trivially-destructible field (stride = 8)
};

struct AnimAlloc { char dummy; };

// Element destroy helper (thiscall in ecx): retail call target 0x5930f3.
void __fastcall AnimPair_Destroy(AnimPair* self);

void __fastcall Destroy_range(AnimPair* first, AnimPair* last, AnimAlloc* al)
{
    (void)al;
    for (; first != last; ++first)
        AnimPair_Destroy(first);
}