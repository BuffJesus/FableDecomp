#include <stdio.h>

enum EHeroMorphType { MORPH_G_DUMMY };

class CParticleMorphs
{
public:
    class CEntry
    {
    public:
        CEntry() : field(9) {}
        int field;
    };
};

template<class _Ty1, class _Ty2>
struct fpair
{
    fpair() : first(), second() {}
    _Ty1 first;
    _Ty2 second;
};

typedef fpair<EHeroMorphType, CParticleMorphs::CEntry> PairT;

struct DummyAllocator
{
    void Cons_val_pair_morph_g(PairT* _Ptr);
};

void DummyAllocator::Cons_val_pair_morph_g(PairT* _Ptr)
{
    _Ptr->fpair<EHeroMorphType, CParticleMorphs::CEntry>::fpair();
}

int main()
{
    DummyAllocator al;
    char raw[sizeof(PairT)];
    for (unsigned i = 0; i < sizeof(PairT); ++i) raw[i] = (char)0xCD;
    PairT* dest = (PairT*)raw;

    al.Cons_val_pair_morph_g(dest);

    if (dest->first == MORPH_G_DUMMY && dest->second.field == 9)
    {
        printf("CONS_VAL_PAIR_MORPH_G_OK\n");
    }
    else
    {
        printf("CONS_VAL_PAIR_MORPH_G_BAD %d %d\n", (int)dest->first, dest->second.field);
    }
    return 0;
}