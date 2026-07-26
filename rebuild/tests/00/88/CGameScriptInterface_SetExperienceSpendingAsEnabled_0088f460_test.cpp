#include <cstdio>
struct CMid { unsigned char _pad[0xd6]; unsigned char flag; };
struct CGameScriptInterface { void* vt; CMid* mid; };
void __fastcall CGameScriptInterface_SetExperienceSpendingAsEnabled(CGameScriptInterface* self, void* /*edx*/, unsigned char enabled) {
    self->mid->flag = enabled;
}
int main() {
    CMid m; m.flag = 0;
    CGameScriptInterface obj; obj.mid = &m;
    CGameScriptInterface_SetExperienceSpendingAsEnabled(&obj, 0, 1);
    if (m.flag != 1) { std::printf("FAIL flag=%d\n", (int)m.flag); return 1; }
    CGameScriptInterface_SetExperienceSpendingAsEnabled(&obj, 0, 0);
    if (m.flag != 0) { std::printf("FAIL flag2=%d\n", (int)m.flag); return 1; }
    std::printf("CGameScriptInterface_0088f460_TEST PASS\n");
    return 0;
}