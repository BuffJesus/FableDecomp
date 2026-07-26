#include <cstdio>

struct CInner {
    int val;
    void Method(int a, int b) { val = a + b; }
};
struct CMid {
    char pad[0x18];
    CInner* p18;
};
struct CGameScriptInterface {
    char pad0[4];
    CMid* p4;
};

void __fastcall CGameScriptInterface_ResetToDefaultTheme(CGameScriptInterface* self, int edx, int arg)
{
    self->p4->p18->Method(1, arg);
}

int main() {
    CInner inner; inner.val = 0;
    CMid theMid; theMid.p18 = &inner;
    CGameScriptInterface obj; obj.p4 = &theMid;
    CGameScriptInterface_ResetToDefaultTheme(&obj, 0, 41);
    if (inner.val == 42) { std::printf("CGameScriptInterface_0088ff10_TEST PASS\n"); return 0; }
    std::printf("FAIL got %d\n", inner.val); return 1;
}