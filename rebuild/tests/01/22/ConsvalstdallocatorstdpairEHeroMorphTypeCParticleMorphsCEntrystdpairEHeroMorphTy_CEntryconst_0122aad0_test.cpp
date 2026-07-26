#include <cstdio>
static int g_AutoTinyFieldCalls = 0;
struct AutoTinyFieldTarget
{
    unsigned int value;
    void Invoke();
};
AutoTinyFieldTarget g_AutoTinyFieldObject = {0};
void AutoTinyFieldTarget::Invoke()
{
    ++g_AutoTinyFieldCalls;
}
void __fastcall Cons_val_std_allocator_std_pair_EHeroMorphType_CParticleMorphs_CEntry_std_pair_EHeroMorphType_CParticleMorphs_CEntry_std_pair_enum_EHeroMorphType_class_CParticleMorphs_CEntry_const()
{
    g_AutoTinyFieldObject.value = 0x0129cbbc;
    g_AutoTinyFieldObject.Invoke();
}
int main()
{
    Cons_val_std_allocator_std_pair_EHeroMorphType_CParticleMorphs_CEntry_std_pair_EHeroMorphType_CParticleMorphs_CEntry_std_pair_enum_EHeroMorphType_class_CParticleMorphs_CEntry_const();
    if (g_AutoTinyFieldObject.value == 0x0129cbbc && g_AutoTinyFieldCalls == 1) { std::printf("AUTO_TINY_0122aad0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122aad0_TEST FAIL\n");
    return 1;
}