struct CQuestMgr {
    void Activate(int quest, int a, int b);
};
extern CQuestMgr* g_questMgr;

void __stdcall CGameScriptInterface_ActivateQuest(int quest)
{
    g_questMgr->Activate(quest, 1, 1);
}