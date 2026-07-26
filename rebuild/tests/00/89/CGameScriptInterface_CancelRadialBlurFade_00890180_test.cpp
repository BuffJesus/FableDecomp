#include <cstdio>

struct CTarget { int called; };
static CTarget* g_last = 0;
void __fastcall Callee(CTarget* self) { g_last = self; if (self) self->called = 1; }

struct CInner {
    char pad[0x1964];
    CTarget* fld;
};
struct CMid {
    char pad[0x18];
    CInner* inner;
};
struct CGameScriptInterface {
    char pad[4];
    CMid* mid;
};

void __fastcall CGameScriptInterface_CancelRadialBlurFade(CGameScriptInterface* self)
{
    Callee(self->mid->inner->fld);
}

int main() {
    CTarget t; t.called = 0;
    CInner innr; innr.fld = &t;
    CMid m; m.inner = &innr;
    CGameScriptInterface gsi; gsi.mid = &m;
    CGameScriptInterface_CancelRadialBlurFade(&gsi);
    if (g_last == &t && t.called == 1) {
        std::printf("CGameScriptInterface_00890180_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}