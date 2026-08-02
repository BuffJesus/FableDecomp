// std::_Cons_val: placement-copy-constructs a std::pair<EHeroMorphType, CParticleMorphs::CEntry>
// value into raw storage, forwarding through the pair's own copy constructor. The allocator
// argument (ecx on entry) is stateless and unused; edx carries the source pair pointer straight
// through untouched, and the destination pointer (the sole stack argument) is loaded into ecx to
// become the "this" of the tail-called pair copy constructor.
struct EHeroMorphType_pair_stub;   // opaque std::pair<EHeroMorphType, CParticleMorphs::CEntry>

extern void __fastcall PairMorphEntry_CopyCtor(
    EHeroMorphType_pair_stub* dest,
    const EHeroMorphType_pair_stub* src);

struct AllocatorMorphPair_stub;    // opaque std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>

void __fastcall Cons_val_pair_morph_h(
    AllocatorMorphPair_stub* /*alloc, unused*/,
    const EHeroMorphType_pair_stub* src,
    EHeroMorphType_pair_stub* dest)
{
    PairMorphEntry_CopyCtor(dest, src);
}