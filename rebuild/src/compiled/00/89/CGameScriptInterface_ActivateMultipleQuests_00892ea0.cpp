struct CQuestMgr {
    void Activate(const void* quests, int a, int b);
};
extern CQuestMgr* g_questMgr;

void __stdcall CGameScriptInterface_ActivateMultipleQuests(const void* quests)
{
    g_questMgr->Activate(quests, 1, 1);
}