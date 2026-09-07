#include "engine/CGameScriptInterface.h"
#include <cstdio>
struct CWorld { unsigned char pad[0xd5]; unsigned char teleporting; };
void __fastcall CGameScriptInterface_SetTeleportingAsActive(CGameScriptInterface* self, int edx, char active) {
    self->World->teleporting = active;
}
int main() {
    CWorld m; m.teleporting = 0;
    CGameScriptInterface o; o.World = &m;
    CGameScriptInterface_SetTeleportingAsActive(&o, 0, 1);
    if (m.teleporting != 1) { std::printf("FAIL set1\n"); return 1; }
    CGameScriptInterface_SetTeleportingAsActive(&o, 0, 0);
    if (m.teleporting != 0) { std::printf("FAIL set0\n"); return 1; }
    std::printf("CGameScriptInterface_0088f410_TEST PASS\n");
    return 0;
}