#include <cstdio>

struct EgoPairMorphImpl
{
    void RealCtor();
};

struct AllocatorConsValPairMorphE
{
    void* field_00;
    EgoPairMorphImpl*** field_04;

    void Construct()
    {
        (**field_04)->RealCtor();
    }
};

void __fastcall Cons_val_pair_morph_e(AllocatorConsValPairMorphE* self, int)
{
    self->Construct();
}

static void* g_lastSelf = 0;

void EgoPairMorphImpl::RealCtor()
{
    g_lastSelf = (void*)this;
}

int main()
{
    EgoPairMorphImpl* realTarget = (EgoPairMorphImpl*)0x12345678;
    EgoPairMorphImpl* level1 = realTarget;
    EgoPairMorphImpl** level2 = &level1;

    AllocatorConsValPairMorphE alloc;
    alloc.field_00 = 0;
    alloc.field_04 = &level2;

    Cons_val_pair_morph_e(&alloc, 0);

    if (g_lastSelf == (void*)realTarget)
    {
        std::printf("TESTOK_CONS_VAL_PAIR_MORPH_E\n");
    }
    else
    {
        std::printf("TESTFAIL\n");
    }
    return 0;
}