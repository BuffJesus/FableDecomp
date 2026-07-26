struct CQuestMgr;
extern CQuestMgr* g_questMgr;
extern void __fastcall QuestMgr_FailAll(CQuestMgr* self);

struct CGameScriptInterface;

void __fastcall CGameScriptInterface_FailAllActiveQuests(CGameScriptInterface* self)
{
    QuestMgr_FailAll(g_questMgr);
}