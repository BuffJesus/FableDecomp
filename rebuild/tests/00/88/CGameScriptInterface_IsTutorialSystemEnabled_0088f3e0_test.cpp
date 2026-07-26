#include <cstdio>
struct TutMgr { char pad0[0xc]; char flagC; char pad1[0x17-0xd]; char flag17; char pad2[0xd7-0x18]; char flagD7; };
static TutMgr g;
extern "C" TutMgr* __cdecl GetTutMgr(void) { return &g; }

char __fastcall CGameScriptInterface_IsTutorialSystemEnabled(void* self)
{
    TutMgr* p = GetTutMgr();
    if (p->flagC)
        return p->flagD7;
    return p->flag17;
}

int main()
{
    g.flagC = 1; g.flagD7 = 7; g.flag17 = 0;
    if (CGameScriptInterface_IsTutorialSystemEnabled(0) != 7) { std::printf("FAIL a\n"); return 1; }
    g.flagC = 0; g.flag17 = 0;
    if (CGameScriptInterface_IsTutorialSystemEnabled(0) != 0) { std::printf("FAIL b\n"); return 1; }
    g.flagC = 0; g.flag17 = 5;
    if (CGameScriptInterface_IsTutorialSystemEnabled(0) != 5) { std::printf("FAIL c\n"); return 1; }
    std::printf("CGameScriptInterface_0088f3e0_TEST PASS\n");
    return 0;
}