#include "rebuild_abi.h"
#include <cstdio>
struct CBuyHost { int v; };
CBuyHost g_bh_b86a10 = {77};
CBuyHost *g_buyHost_b86a10 = &g_bh_b86a10;
void * FABLE_FASTCALL FableBuyHelper_b86a10(CBuyHost *host){ return (void*)(long)host->v; }
struct CAIStateGroup_BuyFromSeller { char pad00[0xc]; void *f0c; void BoughtSomething(); };
void CAIStateGroup_BuyFromSeller::BoughtSomething(){ f0c=FableBuyHelper_b86a10(g_buyHost_b86a10); }
int main(){
    CAIStateGroup_BuyFromSeller s; s.f0c=0; s.BoughtSomething();
    if(s.f0c!=(void*)77){std::printf("BAD\n");return 1;}
    std::printf("BOUGHT_OK\n"); return 0;
}