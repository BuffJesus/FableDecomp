// Retail 0x00c162a0:  add ecx,4 ; jmp 0xc167c0
// _Cons_val<allocator<pair<EHeroMorphType,CParticleMorphs::CEntry> >, ...>
// Base-adjustor member forwarder: 'this' (ecx) is shifted +4 to the embedded
// allocator sub-object, then tail-calls that sub-object's worker with the two
// pointer args passed straight through. Both methods are this-in-ecx (thiscall
// -> fastcall by the harness), so no stack reshuffle: pure add ecx,4 + jmp.

struct Pair;   // std::pair<EHeroMorphType, CParticleMorphs::CEntry>

// Worker at 0xc167c0 operates on the +4 sub-object; declared, not defined.
struct AllocSub {
    void Cons_val_worker(Pair* a, Pair* b);   // this-in-ecx
};

// Outer object: 4-byte lead, then the allocator sub-object at +4.
struct AllocOuter {
    int      lead;   // +0
    AllocSub sub;    // +4
    void _Cons_val_fwd(Pair* a, Pair* b);     // this-in-ecx
};

void AllocOuter::_Cons_val_fwd(Pair* a, Pair* b)
{
    this->sub.Cons_val_worker(a, b);
}