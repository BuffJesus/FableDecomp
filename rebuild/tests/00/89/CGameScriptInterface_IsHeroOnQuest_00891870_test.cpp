#include <cstdio>

struct CQuestMgr { int onQuest; };
CQuestMgr g_mgr = { 1 };
CQuestMgr* g_pQuestMgr = &g_mgr;

extern "C" bool __fastcall QuestMgr_IsHeroOnQuest(CQuestMgr* self)
{
    return self->onQuest != 0;
}

bool __fastcall CGameScriptInterface_IsHeroOnQuest(void* self)
{
    return QuestMgr_IsHeroOnQuest(g_pQuestMgr);
}

int main()
{
    if (CGameScriptInterface_IsHeroOnQuest(0)) {
        std::printf("CGameScriptInterface_00891870_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}