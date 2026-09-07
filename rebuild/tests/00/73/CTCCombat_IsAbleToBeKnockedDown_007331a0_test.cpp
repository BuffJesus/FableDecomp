#include "engine/CTCCombat.h"
#include <cstdio>
struct CThingCreatureBase { char pad[0x9c]; char flag; };
static CThingCreatureBase g_sub;
CThingCreatureBase* __fastcall GetSub(CTCCombat* o) { (void)o; return &g_sub; }

char __fastcall CTCCombat_IsAbleToBeKnockedDown(char* self)
{
    CThingCreatureBase* s = GetSub(*(CTCCombat**)(self + 0x24));
    return s->flag != 0;
}

int main()
{
    char self[0x40];
    CTCCombat owner;
    *(CTCCombat**)(self + 0x24) = &owner;
    g_sub.flag = 0;
    if (CTCCombat_IsAbleToBeKnockedDown(self) != 0) { std::printf("FAIL zero\n"); return 1; }
    g_sub.flag = 7;
    if (CTCCombat_IsAbleToBeKnockedDown(self) != 1) { std::printf("FAIL nonzero\n"); return 1; }
    std::printf("CTCCombat_007331a0_TEST PASS\n");
    return 0;
}