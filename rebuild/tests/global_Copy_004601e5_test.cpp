#include "rebuild_abi.h"
#include <stdio.h>

struct CSpecialAbilitiesTurncoatSpellDef { int x; };

static int g_called = 0;
static CSpecialAbilitiesTurncoatSpellDef* g_this = 0;
static void* g_src = 0;

void __fastcall CSpecialAbilitiesTurncoatSpellDef_Copy_impl(
    CSpecialAbilitiesTurncoatSpellDef* thisptr, void* src)
{
    g_called = 1;
    g_this = thisptr;
    g_src = src;
}

void __fastcall CSpecialAbilitiesTurncoatSpellDef_Copy(
    CSpecialAbilitiesTurncoatSpellDef* thisptr, void* src);

int main(void)
{
    CSpecialAbilitiesTurncoatSpellDef obj;
    obj.x = 7;
    int srcMarker = 0;
    CSpecialAbilitiesTurncoatSpellDef_Copy(&obj, &srcMarker);
    if (g_called && g_this == &obj && g_src == &srcMarker)
        printf("OK_004601e5\n");
    else
        printf("FAIL_004601e5\n");
    return 0;
}