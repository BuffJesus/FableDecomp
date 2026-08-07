#include "rebuild_abi.h"
// CAIStateGroup_BuyFromSeller::BoughtSomething @ 0x00b86a10
// push esi; mov esi,ecx; mov ecx,ds:g_obj; call helper; mov [esi+0xc],eax; ret
// Cache a value fetched via a global-object helper into this->f0c.
struct CBuyHost;
extern CBuyHost *g_buyHost_b86a10;
void * FABLE_FASTCALL FableBuyHelper_b86a10(CBuyHost *host);
struct CAIStateGroup_BuyFromSeller {
    char pad00[0xc];
    void *f0c; // +0xc
    void BoughtSomething();
};
void CAIStateGroup_BuyFromSeller::BoughtSomething()
{
    f0c = FableBuyHelper_b86a10(g_buyHost_b86a10);
}