#include "engine/CGoldDef.h"
#include <cstdio>
struct CDefClassBase { char pad[0x2c]; };
struct CGoldDef_Methods : CGoldDef { void BaseClone(const CDefClassBase* other); };
static int g_baseCalled;
void CGoldDef_Methods::BaseClone(const CDefClassBase*) { g_baseCalled = 1; }
void __fastcall Copy(CGoldDef* self, int, const CDefClassBase* other);
int main()
{
    CGoldDef_Methods src, dst; src.Gold = (long)0xDEADBEEF; dst.Gold = 0;
    Copy(&dst, 0, (const CDefClassBase*)&src);
    if (g_baseCalled == 1 && dst.Gold == (long)0xDEADBEEF) printf("004e0fed_TEST PASS\n");
    else printf("FAIL base=%d f=%08lx\n", g_baseCalled, dst.Gold);
    return 0;
}
