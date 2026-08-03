#include <stdio.h>

enum EHeroMorphType { MORPH_A_DUMMY };

class CParticleMorphs
{
public:
    class CEntry
    {
    public:
        CEntry() : field(7) {}
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
    void Cons_val_pair_morph_a(PairT* _Ptr);
};

void DummyAllocator::Cons_val_pair_morph_a(PairT* _Ptr)
{
    _Ptr->fpair<EHeroMorphType, CParticleMorphs::CEntry>::fpair();
}

int main()
{
    DummyAllocator al;
    char raw[sizeof(PairT)];
    for (unsigned i = 0; i < sizeof(PairT); ++i) raw[i] = (char)0xCD;
    PairT* dest = (PairT*)raw;

    al.Cons_val_pair_morph_a(dest);

    if (dest->first == MORPH_A_DUMMY && dest->second.field == 7)
    {
        printf("CONS_VAL_PAIR_MORPH_A_OK\n");
    }
    else
    {
        printf("CONS_VAL_PAIR_MORPH_A_BAD %d %d\n", (int)dest->first, dest->second.field);
    }
    return 0;
}