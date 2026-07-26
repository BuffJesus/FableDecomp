#include <cstdio>

// Mock the impl so the thunk has something to call.
struct CSpecialAbilitiesElectrocutedEffectDef;
static int g_called = 0;
extern "C" {}

void __fastcall CSpecialAbilitiesElectrocutedEffectDef_Copy_impl(
    CSpecialAbilitiesElectrocutedEffectDef* thisptr, void* other)
{
    (void)thisptr; (void)other;
    g_called = 1;
}

void __fastcall CSpecialAbilitiesElectrocutedEffectDef_Copy(
    CSpecialAbilitiesElectrocutedEffectDef* thisptr, void* other);

int main()
{
    CSpecialAbilitiesElectrocutedEffectDef* self =
        (CSpecialAbilitiesElectrocutedEffectDef*)0;
    int other = 42;
    CSpecialAbilitiesElectrocutedEffectDef_Copy(self, &other);
    if (g_called == 1)
        printf("PASS_460518\n");
    else
        printf("FAIL_460518\n");
    return 0;
}