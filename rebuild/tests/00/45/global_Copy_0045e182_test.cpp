#include <stdio.h>
#include "rebuild_abi.h"

struct COpinionDeedMaskDef;

extern void __fastcall COpinionDeedMaskDef_Copy(COpinionDeedMaskDef* thisptr, void* other);

// Provide the extern target so the test links; the thunk just tail-calls it.
static int g_called = 0;
void __fastcall COpinionDeedMaskDef_Copy_impl(COpinionDeedMaskDef* thisptr, void* other)
{
    (void)thisptr; (void)other;
    g_called = 1;
}

int main()
{
    COpinionDeedMaskDef_Copy((COpinionDeedMaskDef*)0x1000, (void*)0x2000);
    if (g_called == 1)
        printf("OK_45e182_THUNK\n");
    return 0;
}