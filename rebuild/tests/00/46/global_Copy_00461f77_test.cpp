#include <stdio.h>

struct CHeroCombatDef;

extern "C" void __fastcall CHeroCombatDef_Copy_impl(CHeroCombatDef* thisptr, CHeroCombatDef* src);

// Provide the forwarded impl so the test links and runs deterministically.
static int g_called = 0;
extern "C" void __fastcall CHeroCombatDef_Copy_impl(CHeroCombatDef* thisptr, CHeroCombatDef* src)
{
    (void)thisptr; (void)src;
    g_called = 1;
}

void __fastcall CHeroCombatDef_Copy(CHeroCombatDef* thisptr, CHeroCombatDef* src);

int main()
{
    CHeroCombatDef* a = (CHeroCombatDef*)0x1000;
    CHeroCombatDef* b = (CHeroCombatDef*)0x2000;
    CHeroCombatDef_Copy(a, b);
    if (g_called == 1)
        printf("OK_461f77\n");
    else
        printf("FAIL_461f77\n");
    return 0;
}