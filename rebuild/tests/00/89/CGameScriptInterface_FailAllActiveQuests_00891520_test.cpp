#include <cstdio>

struct CQuestMgr { int failed; };
static CQuestMgr g_mgr = { 0 };
CQuestMgr* g_questMgr = &g_mgr;

void __fastcall QuestMgr_FailAll(CQuestMgr* self) { self->failed = 1; }

struct CGameScriptInterface;

void __fastcall CGameScriptInterface_FailAllActiveQuests(CGameScriptInterface* self)
{
    QuestMgr_FailAll(g_questMgr);
}

int main()
{
    g_mgr.failed = 0;
    CGameScriptInterface_FailAllActiveQuests(0);
    if (g_mgr.failed == 1) {
        std::printf("CGameScriptInterface_00891520_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}