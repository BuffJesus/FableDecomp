#include "rebuild_abi.h"
#include <stdio.h>

struct CSpecialAbilitiesForcePushDef;

// Provide the impl so the test links; record that the tail-call reached it.
static int g_called = 0;
static CSpecialAbilitiesForcePushDef* g_this = 0;
static const void* g_src = 0;

void __fastcall CSpecialAbilitiesForcePushDef_Copy_impl(
    CSpecialAbilitiesForcePushDef* thisptr, void* /*edx*/, const void* src)
{
    g_called = 1;
    g_this = thisptr;
    g_src = src;
}

extern void __fastcall CSpecialAbilitiesForcePushDef_Copy(
    CSpecialAbilitiesForcePushDef* thisptr, void* edx, const void* src);

int main(void)
{
    CSpecialAbilitiesForcePushDef* self = (CSpecialAbilitiesForcePushDef*)0x1234;
    const void* src = (const void*)0x5678;
    CSpecialAbilitiesForcePushDef_Copy(self, 0, src);
    if (g_called && g_this == self && g_src == src)
        printf("OK_45FD4E\n");
    else
        printf("FAIL_45FD4E\n");
    return 0;
}