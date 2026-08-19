// _Destroy_range<allocator<pair<unsigned long, CAnimationEntry> > >
// retail 0x004dbe58 : __fastcall(first, last, allocator*), stride 0x1c, ret 4
// element = pair<unsigned long, CAnimationEntry>, sizeof == 28 (0x1c).
// The loop invokes the element's destructor once per element: a __fastcall call
// (ecx = element, offset 0) to an out-of-line body (retail 0xa01c10).
//
// Modelled with a declared-only __fastcall member so the codegen is a single
// external `call` (relocation) per element and no local `??_G`/`??1` bodies are
// emitted -- keeping this translation unit's only function the destroy loop, and
// matching retail's `mov ecx,esi ; call <dtor>` exactly.

struct CAnimationEntry
{
    unsigned long a;
    unsigned long b;
    unsigned long c;
    unsigned long d;
    unsigned long e;
    void* p;
};

// stand-in for pair<unsigned long, CAnimationEntry>; DestroyElement is the
// element destructor thunk (defined out-of-line, retail 0xa01c10).
struct pair_ulong_anim
{
    unsigned long first;
    CAnimationEntry second;
    void DestroyElement();   // __fastcall, defined elsewhere
};

struct allocator_pair {};

#pragma optimize("s", on)
void __fastcall Destroy_range_pair_CAnimationEntry(pair_ulong_anim* first,
                                                   pair_ulong_anim* last,
                                                   allocator_pair* al)
{
    (void)al;
    while (first != last)
    {
        first->DestroyElement();
        ++first;
    }
}
#pragma optimize("", on)