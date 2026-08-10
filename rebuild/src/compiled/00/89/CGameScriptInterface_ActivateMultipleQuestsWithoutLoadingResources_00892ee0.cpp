struct CCharStringArray;
struct CQuestMgr {
    void Activate(CCharStringArray const& quests, int a, int b);
};
extern CQuestMgr* g_questMgr;

void __stdcall CGameScriptInterface_ActivateMultipleQuestsWithoutLoadingResources(CCharStringArray const& quests)
{
    g_questMgr->Activate(quests, 1, 0);
}