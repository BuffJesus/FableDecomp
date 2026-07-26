#include "rebuild_abi.h"
#include <stdio.h>

/* Mock the target so the tail-call links; verify the thunk forwards args. */
struct CMeleeCombatKnockdownEffects { int tag; };

static CMeleeCombatKnockdownEffects* g_this = 0;
static void* g_src = 0;

extern void __fastcall CMeleeCombatKnockdownEffects_Copy_impl(
    CMeleeCombatKnockdownEffects* thisptr, void* src);

void __fastcall CMeleeCombatKnockdownEffects_Copy_impl(
    CMeleeCombatKnockdownEffects* thisptr, void* src)
{
    g_this = thisptr;
    g_src = src;
}

extern void __fastcall CMeleeCombatKnockdownEffects_Copy(
    CMeleeCombatKnockdownEffects* thisptr, void* src);

int main()
{
    CMeleeCombatKnockdownEffects obj; obj.tag = 0x11;
    int srcbuf = 0x22;
    CMeleeCombatKnockdownEffects_Copy(&obj, &srcbuf);
    if (g_this == &obj && g_src == &srcbuf)
        printf("PASS_004625f8\n");
    else
        printf("FAIL_004625f8\n");
    return 0;
}