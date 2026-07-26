struct QuestMgr;
extern void __fastcall QuestMgr_RemoveAll(QuestMgr* self);

struct GameObj;
extern GameObj* g_gameObj;

void __fastcall CGameScriptInterface_RemoveAllQuestInfoElements(void* self)
{
    QuestMgr_RemoveAll(*(QuestMgr**)((char*)g_gameObj + 0x1c8));
}