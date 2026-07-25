#include <cstdio>
#include "rebuild_abi.h"

struct CSpecialAbilitiesGhostSwordDef { int x; };

static int g_called = 0;
static CSpecialAbilitiesGhostSwordDef* g_this = 0;
static void* g_src = 0;

extern void __fastcall CSpecialAbilitiesGhostSwordDef_Copy(
    CSpecialAbilitiesGhostSwordDef* thisptr, void* edx, void* src);

// Provide the impl the thunk tail-calls.
void __fastcall CSpecialAbilitiesGhostSwordDef_Copy_impl(
    CSpecialAbilitiesGhostSwordDef* thisptr, void* edx, void* src)
{
    (void)edx;
    g_called = 1;
    g_this = thisptr;
    g_src = src;
}

int main()
{
    CSpecialAbilitiesGhostSwordDef obj;
    obj.x = 42;
    int localSrc = 7;
    CSpecialAbilitiesGhostSwordDef_Copy(&obj, 0, &localSrc);
    if (g_called == 1 && g_this == &obj && g_src == &localSrc)
        printf("OK_460180\n");
    else
        printf("FAIL_460180\n");
    return 0;
}