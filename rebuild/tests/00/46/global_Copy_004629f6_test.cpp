#include "rebuild_abi.h"
#include <cstdio>

struct CEngineThemeDef;

extern void __fastcall CEngineThemeDef_Copy(CEngineThemeDef* thisptr, CEngineThemeDef* src);

// Provide the impl the thunk jumps to so the test links & runs.
static int g_called = 0;
void __fastcall CEngineThemeDef_Copy_impl(CEngineThemeDef* thisptr, CEngineThemeDef* src)
{
    (void)thisptr; (void)src;
    g_called = 1;
}

int main()
{
    CEngineThemeDef_Copy((CEngineThemeDef*)0x1000, (CEngineThemeDef*)0x2000);
    if (g_called == 1)
        printf("OK_004629f6\n");
    else
        printf("FAIL_004629f6\n");
    return 0;
}