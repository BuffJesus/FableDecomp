struct CQuestMgr;
extern CQuestMgr* g_pQuestMgr;
extern "C" bool __fastcall QuestMgr_IsHeroOnQuest(CQuestMgr* self);

bool __fastcall CGameScriptInterface_IsHeroOnQuest(void* self)
{
    return QuestMgr_IsHeroOnQuest(g_pQuestMgr);
}