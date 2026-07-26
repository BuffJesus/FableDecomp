#include "rebuild_abi.h"
#include <cstdio>

struct CMeleeCombatAbilityDef;

extern void __fastcall CMeleeCombatAbilityDef_Copy_impl(CMeleeCombatAbilityDef* thisptr, void* other);
extern void __fastcall CMeleeCombatAbilityDef_Copy(CMeleeCombatAbilityDef* thisptr, void* other);

static int g_called = 0;
void __fastcall CMeleeCombatAbilityDef_Copy_impl(CMeleeCombatAbilityDef* thisptr, void* other)
{
    (void)thisptr; (void)other;
    g_called = 1;
}

int main()
{
    CMeleeCombatAbilityDef_Copy((CMeleeCombatAbilityDef*)0x1000, (void*)0x2000);
    if (g_called == 1)
        printf("PASS_0046244c\n");
    return 0;
}