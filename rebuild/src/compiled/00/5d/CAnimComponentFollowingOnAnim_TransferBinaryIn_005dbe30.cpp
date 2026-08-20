// Retail 0x00996bc0:
//   add ecx, 8
//   jmp  0x997330
// A _Cons_val forwarder for allocator< pair<EHeroMorphType,CParticleMorphs::CEntry> >.
// The outer allocator embeds the real element-allocator subobject at +8; the
// forwarder shifts `this` to that subobject and tail-calls its _Cons_val.

enum EHeroMorphType { HMT_NONE };

struct CEntry {
    int a;
    int b;
    int c;
};

struct Pair {
    EHeroMorphType first;
    CEntry second;
};

// The inner element-allocator subobject (lives at outer+8).
struct InnerAlloc {
    // real target at 0x997330 (declared, resolved by linker/relocation)
    void Cons_val(Pair* p, const Pair* v);
};

// Outer allocator: 8 bytes of preceding state, then the inner subobject at +8.
struct OuterAlloc {
    int pad0;
    int pad4;
    InnerAlloc inner;   // at +8

    void Cons_val(Pair* p, const Pair* v);
};

void OuterAlloc::Cons_val(Pair* p, const Pair* v)
{
    // ecx += 8 (adjust to inner), then tail-jmp into inner.Cons_val
    inner.Cons_val(p, v);
}