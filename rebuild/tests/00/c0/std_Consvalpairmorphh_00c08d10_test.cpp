#include <stdio.h>

struct EHeroMorphType_pair_stub { int a; int b; int c; };

int g_copyCtorCalls = 0;
const EHeroMorphType_pair_stub* g_lastSrc = 0;
EHeroMorphType_pair_stub* g_lastDest = 0;

void __fastcall PairMorphEntry_CopyCtor(
    EHeroMorphType_pair_stub* dest,
    const EHeroMorphType_pair_stub* src)
{
    g_copyCtorCalls++;
    g_lastDest = dest;
    g_lastSrc = src;
    *dest = *src;
}

struct AllocatorMorphPair_stub { int unused; };

void __fastcall Cons_val_pair_morph_h(
    AllocatorMorphPair_stub* alloc,
    const EHeroMorphType_pair_stub* src,
    EHeroMorphType_pair_stub* dest)
{
    PairMorphEntry_CopyCtor(dest, src);
}

int main()
{
    AllocatorMorphPair_stub alloc;
    EHeroMorphType_pair_stub source;
    source.a = 11; source.b = 22; source.c = 33;

    EHeroMorphType_pair_stub dest;
    dest.a = 0; dest.b = 0; dest.c = 0;

    Cons_val_pair_morph_h(&alloc, &source, &dest);

    bool ok = (g_copyCtorCalls == 1)
        && (g_lastDest == &dest)
        && (g_lastSrc == &source)
        && (dest.a == 11) && (dest.b == 22) && (dest.c == 33);

    if (ok)
        puts("PASS_00c08d10_cons_val_pair_morph");
    else
        puts("FAIL_00c08d10_cons_val_pair_morph");
    return 0;
}