#include "rebuild_abi.h"
#include <stdio.h>

struct CSpecialAbilitiesBerserkDef;

extern void __fastcall CSpecialAbilitiesBerserkDef_Copy_impl(
    CSpecialAbilitiesBerserkDef* thisptr, void* src);
extern void __fastcall CSpecialAbilitiesBerserkDef_Copy(
    CSpecialAbilitiesBerserkDef* thisptr, void* src);

// Provide a definition for the tail-call target so the test links and runs.
static int g_called = 0;
void __fastcall CSpecialAbilitiesBerserkDef_Copy_impl(
    CSpecialAbilitiesBerserkDef* thisptr, void* src)
{
    (void)thisptr; (void)src;
    g_called = 1;
}

int main()
{
    CSpecialAbilitiesBerserkDef_Copy((CSpecialAbilitiesBerserkDef*)0x1000, (void*)0x2000);
    if (g_called == 1)
        printf("OK_0045f84f\n");
    else
        printf("FAIL_0045f84f\n");
    return 0;
}