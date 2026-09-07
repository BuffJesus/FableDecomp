#include "engine/CGameScriptInterface.h"
#include <cstdio>
struct CWorld {
    unsigned char _pad[0xdb];
    unsigned char allowScreenFadingOnNextRegionChange; // +0xdb
};
void __fastcall CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange(
        CGameScriptInterface* self, int edx_dummy, char val) {
    self->World->allowScreenFadingOnNextRegionChange = (unsigned char)val;
}
int main() {
    CWorld midObj;
    CGameScriptInterface obj;
    CWorld* mp = &midObj;
    obj.World = mp;
    midObj.allowScreenFadingOnNextRegionChange = 0;
    CGameScriptInterface* op = &obj;
    CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange(op, 0, 1);
    if (midObj.allowScreenFadingOnNextRegionChange != 1) { std::printf("FAIL set to 1\n"); return 1; }
    CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange(op, 0, 0);
    if (midObj.allowScreenFadingOnNextRegionChange != 0) { std::printf("FAIL set to 0\n"); return 1; }
    std::printf("CGameScriptInterface_0088e910_TEST PASS\n");
    return 0;
}