#include <cstdio>

struct QuestMgr { int removed; };
struct GameObj { char pad[0x1c8]; QuestMgr* questMgr; };

static QuestMgr g_mgr = { 0 };
static GameObj g_obj_storage;
GameObj* g_gameObj = &g_obj_storage;

void __fastcall QuestMgr_RemoveAll(QuestMgr* self) { self->removed = 1; }

void __fastcall CGameScriptInterface_RemoveAllQuestInfoElements(void* self)
{
    QuestMgr_RemoveAll(*(QuestMgr**)((char*)g_gameObj + 0x1c8));
}

int main()
{
    g_obj_storage.questMgr = &g_mgr;
    g_mgr.removed = 0;
    CGameScriptInterface_RemoveAllQuestInfoElements(0);
    if (g_mgr.removed == 1) {
        std::printf("CGameScriptInterface_00891c30_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL removed=%d\n", g_mgr.removed);
    return 1;
}