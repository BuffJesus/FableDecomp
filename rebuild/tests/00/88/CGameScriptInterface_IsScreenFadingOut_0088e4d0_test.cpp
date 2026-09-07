#include "engine/CGameScriptInterface.h"
#include <cstdio>

struct CDisplayEngine { char pad0[0xbc]; unsigned char b_bc; char pad1[0xc]; unsigned char b_c9; };

int __fastcall CGameScriptInterface_IsScreenFadingOut(const CGameScriptInterface* self)
{
    const CDisplayEngine* s = self->DisplayEngine;
    if (s->b_bc) {
        if (s->b_c9) {
            return 1;
        }
    }
    return 0;
}

int main()
{
    CDisplayEngine node;
    CGameScriptInterface obj;
    obj.DisplayEngine = &node;
    const CGameScriptInterface* p = &obj;

    node.b_bc = 1; node.b_c9 = 1;
    if (CGameScriptInterface_IsScreenFadingOut(p) != 1) { std::printf("FAIL both-set\n"); return 1; }

    node.b_bc = 1; node.b_c9 = 0;
    if (CGameScriptInterface_IsScreenFadingOut(p) != 0) { std::printf("FAIL c9-zero\n"); return 1; }

    node.b_bc = 0; node.b_c9 = 1;
    if (CGameScriptInterface_IsScreenFadingOut(p) != 0) { std::printf("FAIL bc-zero\n"); return 1; }

    node.b_bc = 0; node.b_c9 = 0;
    if (CGameScriptInterface_IsScreenFadingOut(p) != 0) { std::printf("FAIL both-zero\n"); return 1; }

    std::printf("CGameScriptInterface_0088e4d0_TEST PASS\n");
    return 0;
}