#include "engine/CTavernDef.h"
#include <cstdio>
struct CDefClassBase;
struct CTavernDef_Methods : CTavernDef { void base_clone(const CDefClassBase* src); };
static void* g_self; static const void* g_src;
void CTavernDef_Methods::base_clone(const CDefClassBase* src) { g_self = this; g_src = src; }
void __fastcall CTavernDef_Copy(CTavernDef* self, int, const CDefClassBase* src);
int main()
{
    CTavernDef_Methods dst; dst.BedCost = 0;
    CTavernDef_Methods srcObj; srcObj.BedCost = 0x12345678;
    CTavernDef_Copy(&dst, 0, (const CDefClassBase*)&srcObj);
    if (dst.BedCost == 0x12345678 && g_self == &dst && g_src == &srcObj) printf("004e0f02_TEST PASS\n");
    else printf("FAIL %x\n", dst.BedCost);
    return 0;
}
