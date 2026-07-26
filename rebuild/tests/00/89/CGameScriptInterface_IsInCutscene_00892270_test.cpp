#include <cstdio>
struct CGameScriptInterface { char pad; };
struct CutsceneMgr { char pad[0xf6]; unsigned char inCutscene; };
CutsceneMgr g_mgr;
CutsceneMgr* g_cutsceneMgr = &g_mgr;
char __fastcall CGameScriptInterface_IsInCutscene(const CGameScriptInterface* self)
{
    return (char)g_cutsceneMgr->inCutscene;
}
int main()
{
    CGameScriptInterface obj;
    g_mgr.inCutscene = 0;
    if (CGameScriptInterface_IsInCutscene(&obj) != 0) { std::printf("FAIL a\n"); return 1; }
    g_mgr.inCutscene = 1;
    if (CGameScriptInterface_IsInCutscene(&obj) != 1) { std::printf("FAIL b\n"); return 1; }
    std::printf("CGameScriptInterface_00892270_TEST PASS\n");
    return 0;
}