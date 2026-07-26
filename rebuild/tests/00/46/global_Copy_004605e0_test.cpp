#include "rebuild_abi.h"
#include <stdio.h>

struct CSpecialAbilitiesThunderLightningStormDef { int dummy; };

// Provide a definition for the "impl" target so the test links & runs.
static int g_called = 0;
static CSpecialAbilitiesThunderLightningStormDef* g_this = 0;
static void* g_src = 0;

extern "C" void __fastcall CSpecialAbilitiesThunderLightningStormDef_Copy_impl(
    CSpecialAbilitiesThunderLightningStormDef* thisptr, void* src)
{
    g_called = 1;
    g_this = thisptr;
    g_src = src;
}

void __fastcall CSpecialAbilitiesThunderLightningStormDef_Copy(
    CSpecialAbilitiesThunderLightningStormDef* thisptr, void* src);

int main(void)
{
    CSpecialAbilitiesThunderLightningStormDef obj;
    obj.dummy = 42;
    int srcMarker = 7;

    CSpecialAbilitiesThunderLightningStormDef_Copy(&obj, &srcMarker);

    if (g_called == 1 && g_this == &obj && g_src == &srcMarker)
        printf("OK_THUNK_004605e0\n");
    else
        printf("FAIL_004605e0\n");
    return 0;
}