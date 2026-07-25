#include "rebuild_abi.h"
#include <cstdio>

struct CPlayerGuiDef { int dummy; };

extern void __fastcall CPlayerGuiDef_Copy(CPlayerGuiDef* thisptr, void* other);

// Provide the impl so the test links; it just records the call.
static int g_called = 0;
static CPlayerGuiDef* g_this = 0;
static void* g_other = 0;
void __fastcall CPlayerGuiDef_Copy_impl(CPlayerGuiDef* thisptr, void* other)
{
    g_called = 1;
    g_this = thisptr;
    g_other = other;
}

int main()
{
    CPlayerGuiDef a; a.dummy = 7;
    int src = 42;
    CPlayerGuiDef_Copy(&a, &src);
    if (g_called && g_this == &a && g_other == &src)
        printf("OK_0046ce28\n");
    else
        printf("FAIL_0046ce28\n");
    return 0;
}