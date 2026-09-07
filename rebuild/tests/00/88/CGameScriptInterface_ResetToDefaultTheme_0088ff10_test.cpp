#include "engine/CGameScriptInterface.h"
#include <cstdio>

struct CInner {
    int val;
    void Method(int a, int b) { val = a + b; }
};
struct CWorld {
    char pad[0x18];
    CInner* p18;
};

void __fastcall CGameScriptInterface_ResetToDefaultTheme(CGameScriptInterface* self, int edx, int arg)
{
    self->World->p18->Method(1, arg);
}

int main() {
    CInner inner; inner.val = 0;
    CWorld theMid; theMid.p18 = &inner;
    CGameScriptInterface obj; obj.World = &theMid;
    CGameScriptInterface_ResetToDefaultTheme(&obj, 0, 41);
    if (inner.val == 42) { std::printf("CGameScriptInterface_0088ff10_TEST PASS\n"); return 0; }
    std::printf("FAIL got %d\n", inner.val); return 1;
}