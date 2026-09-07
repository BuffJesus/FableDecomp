#include "engine/CGameScriptInterface.h"
#include <cstdio>

struct CTarget { int called; };
static CTarget* g_last = 0;
void __fastcall Callee(CTarget* self) { g_last = self; if (self) self->called = 1; }

struct CInner {
    char pad[0x1964];
    CTarget* fld;
};
struct CWorld {
    char pad[0x18];
    CInner* inner;
};

void __fastcall CGameScriptInterface_CancelRadialBlurFade(CGameScriptInterface* self)
{
    Callee(self->World->inner->fld);
}

int main() {
    CTarget t; t.called = 0;
    CInner innr; innr.fld = &t;
    CWorld m; m.inner = &innr;
    CGameScriptInterface gsi; gsi.World = &m;
    CGameScriptInterface_CancelRadialBlurFade(&gsi);
    if (g_last == &t && t.called == 1) {
        std::printf("CGameScriptInterface_00890180_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}