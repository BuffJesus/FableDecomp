#include "engine/CGameScriptInterface.h"
#include <cstdio>
struct Inner2 { unsigned char _p[0x14]; long day; };
struct CWorld { unsigned char _p[0x1c]; Inner2* p1c; };

long __fastcall CGameScriptInterface_GetDayCount(const CGameScriptInterface* self) {
    return self->World->p1c->day;
}

int main() {
    Inner2 leaf; leaf.day = 42;
    Inner2* pLeaf = &(leaf);
    CWorld mid; mid.p1c = pLeaf;
    CWorld* pMid = &(mid);
    CGameScriptInterface obj; obj.World = pMid;
    CGameScriptInterface* pObj = &(obj);
    long r = CGameScriptInterface_GetDayCount(pObj);
    if (r == 42) { std::printf("CGameScriptInterface_0088fe70_TEST PASS\n"); return 0; }
    std::printf("FAIL got %ld\n", r); return 1;
}