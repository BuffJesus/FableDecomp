#include <cstdio>
#include "rebuild_abi.h"

struct CSpecialAbilitiesDrainLifeDef;

static int g_called = 0;
extern "C" void __fastcall CSpecialAbilitiesDrainLifeDef_Copy_base(
    CSpecialAbilitiesDrainLifeDef* thisptr, void* src)
{
    (void)thisptr; (void)src;
    g_called = 1;
}

void __fastcall CSpecialAbilitiesDrainLifeDef_Copy(
    CSpecialAbilitiesDrainLifeDef* thisptr, void* src);

int main()
{
    CSpecialAbilitiesDrainLifeDef* obj = (CSpecialAbilitiesDrainLifeDef*)0x1000;
    void* src = (void*)0x2000;
    CSpecialAbilitiesDrainLifeDef_Copy(obj, src);
    if (g_called == 1)
        printf("PASS_004600c6\n");
    return 0;
}