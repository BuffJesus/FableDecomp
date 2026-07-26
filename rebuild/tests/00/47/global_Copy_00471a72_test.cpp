#include <cstdio>
#include "rebuild_abi.h"

struct COpinionDeedEffectsDef;

extern void __fastcall COpinionDeedEffectsDef_Copy(COpinionDeedEffectsDef* thisptr, void* src);

/* The real callee is relocation-masked; stub it so the test links & runs. */
static int g_called = 0;
extern void __fastcall COpinionDeedEffectsDef_Copy_impl(COpinionDeedEffectsDef* thisptr, void* src);
void __fastcall COpinionDeedEffectsDef_Copy_impl(COpinionDeedEffectsDef* thisptr, void* src)
{
    (void)thisptr; (void)src;
    g_called = 1;
}

int main()
{
    int obj = 0;
    int arg = 0;
    COpinionDeedEffectsDef_Copy((COpinionDeedEffectsDef*)&obj, &arg);
    if (g_called == 1)
        printf("OK_471a72\n");
    return 0;
}