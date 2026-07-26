#include "rebuild_abi.h"
#include <cstdio>

struct CCameraManagerDef;

extern void __fastcall CCameraManagerDef_Copy(CCameraManagerDef* thisptr, void* other);

// Provide the forwarded impl so the test links; records the call.
static int g_called = 0;
void __fastcall CCameraManagerDef_Copy_impl(CCameraManagerDef* thisptr, void* other)
{
    (void)thisptr; (void)other;
    g_called = 1;
}

int main()
{
    CCameraManagerDef_Copy((CCameraManagerDef*)0x1000, (void*)0x2000);
    if (g_called == 1)
        printf("OK_0045d500\n");
    else
        printf("FAIL_0045d500\n");
    return 0;
}