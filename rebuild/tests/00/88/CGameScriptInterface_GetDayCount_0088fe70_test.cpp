#include <cstdio>
struct Inner2 { unsigned char _p[0x14]; long day; };
struct CMid { unsigned char _p[0x1c]; Inner2* p1c; };
struct CGameScriptInterface { void* vt; CMid* mid; };

long __fastcall CGameScriptInterface_GetDayCount(const CGameScriptInterface* self) {
    return self->mid->p1c->day;
}

int main() {
    Inner2 leaf; leaf.day = 42;
    Inner2* pLeaf = &(leaf);
    CMid mid; mid.p1c = pLeaf;
    CMid* pMid = &(mid);
    CGameScriptInterface obj; obj.mid = pMid;
    CGameScriptInterface* pObj = &(obj);
    long r = CGameScriptInterface_GetDayCount(pObj);
    if (r == 42) { std::printf("CGameScriptInterface_0088fe70_TEST PASS\n"); return 0; }
    std::printf("FAIL got %ld\n", r); return 1;
}