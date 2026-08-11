#pragma optimize("s",on)
// std::_Destroy_range<allocator<pair<unsigned long, CAnimationEntry> > >
// __fastcall(first /*ecx*/, last /*edx*/, allocator* /*stack, unused*/) -> void
// Loops [first,last) in stride-8 steps, destroying each element.

struct Elem {            // sizeof == 8 : pair<unsigned long, CAnimationEntry>
    unsigned long first;
    unsigned long second;
};

struct Alloc { char dummy; };

// External per-element destroy helper (this-in-ecx, single arg). Retail 0x5f8736.
void __fastcall Destroy_pair(Elem* p);

void __fastcall Destroy_range(Elem* first, Elem* last, Alloc* al)
{
    for (; first != last; ++first)
        Destroy_pair(first);
    (void)al;
}