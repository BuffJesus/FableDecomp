#include "rebuild_abi.h"
#include <cstdio>

struct CSpecialAbilitiesBulletTimeDef;
struct CDefClassBase;

static int g_hit = 0;

extern "C" void __fastcall CSpecialAbilitiesBulletTimeDef_Copy_impl(
    CSpecialAbilitiesBulletTimeDef* thisptr, const CDefClassBase* src)
{
    (void)thisptr; (void)src;
    g_hit = 1;
}

void __fastcall CSpecialAbilitiesBulletTimeDef_Copy(
    CSpecialAbilitiesBulletTimeDef* thisptr, const CDefClassBase* src);

int main(void)
{
    CSpecialAbilitiesBulletTimeDef_Copy(
        (CSpecialAbilitiesBulletTimeDef*)0x1000,
        (const CDefClassBase*)0x2000);
    if (g_hit == 1)
        printf("OK_THUNK_0045fabc\n");
    return 0;
}