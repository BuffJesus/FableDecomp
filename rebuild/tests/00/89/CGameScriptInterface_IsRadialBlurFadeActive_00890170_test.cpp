#include "engine/CGameScriptInterface.h"
#include <cstdio>

struct Inner { char pad[0x78]; bool flag; };
struct Mid { char pad[0x1964]; Inner* inner; };
struct CWorld { char pad[0x18]; Mid* mid; };

bool __fastcall CGameScriptInterface_IsRadialBlurFadeActive(const CGameScriptInterface* self)
{
    return self->World->mid->inner->flag;
}

int main()
{
    Inner leaf;
    leaf.flag = true;
    Mid midv;
    midv.inner = (&leaf);
    CWorld outv;
    outv.mid = (&midv);
    CGameScriptInterface selfv;
    selfv.World = (&outv);
    bool r = CGameScriptInterface_IsRadialBlurFadeActive((&selfv));
    leaf.flag = false;
    bool r2 = CGameScriptInterface_IsRadialBlurFadeActive((&selfv));
    if (r == true && r2 == false) {
        std::printf("CGameScriptInterface_00890170_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL r=%d r2=%d\n", (int)r, (int)r2);
    return 1;
}