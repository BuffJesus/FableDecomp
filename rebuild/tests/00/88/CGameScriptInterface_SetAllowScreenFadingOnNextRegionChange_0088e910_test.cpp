#include <cstdio>
struct CMid {
    unsigned char _pad[0xdb];
    unsigned char allowScreenFadingOnNextRegionChange; // +0xdb
};
struct CGameScriptInterface {
    void* vt;      // +0x0
    CMid* mid;     // +0x4
};
void __fastcall CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange(
        CGameScriptInterface* self, int edx_dummy, char val) {
    self->mid->allowScreenFadingOnNextRegionChange = (unsigned char)val;
}
int main() {
    CMid midObj;
    CGameScriptInterface obj;
    CMid* mp = &midObj;
    obj.mid = mp;
    midObj.allowScreenFadingOnNextRegionChange = 0;
    CGameScriptInterface* op = &obj;
    CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange(op, 0, 1);
    if (midObj.allowScreenFadingOnNextRegionChange != 1) { std::printf("FAIL set to 1\n"); return 1; }
    CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange(op, 0, 0);
    if (midObj.allowScreenFadingOnNextRegionChange != 0) { std::printf("FAIL set to 0\n"); return 1; }
    std::printf("CGameScriptInterface_0088e910_TEST PASS\n");
    return 0;
}