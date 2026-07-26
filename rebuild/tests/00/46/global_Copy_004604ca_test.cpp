#include "rebuild_abi.h"
#include <stdio.h>

struct CSpecialAbilitiesBurntEffectDef { int dummy; };

// Provide the impl target so the thunk links; record that it ran.
static int g_called = 0;
static CSpecialAbilitiesBurntEffectDef* g_this = 0;
static void* g_src = 0;

extern void __fastcall CSpecialAbilitiesBurntEffectDef_Copy_impl(
    CSpecialAbilitiesBurntEffectDef* thisptr, void* src);

void __fastcall CSpecialAbilitiesBurntEffectDef_Copy_impl(
    CSpecialAbilitiesBurntEffectDef* thisptr, void* src)
{
    g_called = 1;
    g_this = thisptr;
    g_src = src;
}

extern void __fastcall CSpecialAbilitiesBurntEffectDef_Copy(
    CSpecialAbilitiesBurntEffectDef* thisptr, void* src);

int main(void)
{
    CSpecialAbilitiesBurntEffectDef obj;
    obj.dummy = 7;
    int srcval = 42;
    CSpecialAbilitiesBurntEffectDef_Copy(&obj, &srcval);

    if (g_called == 1 && g_this == &obj && g_src == &srcval)
        printf("OK_004604ca\n");
    else
        printf("FAIL_004604ca\n");
    return 0;
}