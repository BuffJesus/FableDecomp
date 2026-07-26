#include "rebuild_abi.h"
#include <cstdio>

struct CCombatDialogueDef { int x; };

// Provide the impl the thunk tail-jumps to.
static int g_called = 0;
static CCombatDialogueDef* g_this = 0;
static void* g_src = 0;

extern "C" void __fastcall CCombatDialogueDef_Copy_impl_c(CCombatDialogueDef* thisptr, void* src)
{
    g_called = 1;
    g_this = thisptr;
    g_src = src;
}

// Alias to the C++-mangled name the thunk references.
void __fastcall CCombatDialogueDef_Copy_impl(CCombatDialogueDef* thisptr, void* src)
{
    CCombatDialogueDef_Copy_impl_c(thisptr, src);
}

void __fastcall CCombatDialogueDef_Copy(CCombatDialogueDef* thisptr, void* src);

int main()
{
    CCombatDialogueDef obj;
    obj.x = 0x45E555;
    int marker = 0;
    CCombatDialogueDef_Copy(&obj, &marker);

    if (g_called && g_this == &obj && g_src == (void*)&marker)
        printf("PASS_0045e555_THUNK\n");
    else
        printf("FAIL_0045e555\n");
    return 0;
}