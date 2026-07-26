#include <cstdio>
#include "rebuild_abi.h"

struct CSpecialAbilitiesBattleChargeDef { int x; };

// Stand-in impl so the test links & runs deterministically.
extern "C" { static int g_called = 0; }
void __fastcall CSpecialAbilitiesBattleChargeDef_Copy_impl(
    CSpecialAbilitiesBattleChargeDef* thisptr, void* src)
{
    (void)thisptr; (void)src; g_called = 1;
}

extern void __fastcall CSpecialAbilitiesBattleChargeDef_Copy(
    CSpecialAbilitiesBattleChargeDef* thisptr, void* src);

int main(void)
{
    CSpecialAbilitiesBattleChargeDef dst; dst.x = 0;
    int srcbuf = 7;
    CSpecialAbilitiesBattleChargeDef_Copy(&dst, &srcbuf);
    if (g_called == 1)
        printf("OK_045f630\n");
    else
        printf("FAIL_045f630\n");
    return 0;
}