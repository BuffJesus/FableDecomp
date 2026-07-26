struct AutoTinyMethodTarget
{
    void Invoke();
};
extern AutoTinyMethodTarget g_AutoTinyMethodObject;
extern unsigned int g_AutoTinyMethodCounter;
void __fastcall Cons_val_std_allocator_std_pair_EHeroMorphType_CParticleMorphs_CEntry_std_pair_EHeroMorphType_CParticleMorphs_CEntry_std_pair_enum_EHeroMorphType_class_CParticleMorphs_CEntry_const()
{
    g_AutoTinyMethodObject.Invoke();
    --g_AutoTinyMethodCounter;
}