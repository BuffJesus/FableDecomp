#include <cstdio>
static int g_AutoTinyMethodCalls = 0;
struct AutoTinyMethodTarget
{
    void Invoke();
};
AutoTinyMethodTarget g_AutoTinyMethodObject;
unsigned int g_AutoTinyMethodCounter = 2;
void AutoTinyMethodTarget::Invoke()
{
    ++g_AutoTinyMethodCalls;
}
void __fastcall Cons_val_std_allocator_std_pair_EHeroMorphType_CParticleMorphs_CEntry_std_pair_EHeroMorphType_CParticleMorphs_CEntry_std_pair_enum_EHeroMorphType_class_CParticleMorphs_CEntry_const()
{
    g_AutoTinyMethodObject.Invoke();
    --g_AutoTinyMethodCounter;
}
int main()
{
    Cons_val_std_allocator_std_pair_EHeroMorphType_CParticleMorphs_CEntry_std_pair_EHeroMorphType_CParticleMorphs_CEntry_std_pair_enum_EHeroMorphType_class_CParticleMorphs_CEntry_const();
    if (g_AutoTinyMethodCalls == 1 && g_AutoTinyMethodCounter == 1) { std::printf("AUTO_TINY_0122a5e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a5e0_TEST FAIL\n");
    return 1;
}