#include <cstdio>

static int g_a, g_b, g_quest, g_called;
struct CQuestMgr {
    void Activate(int quest, int a, int b) {
        g_quest = quest; g_a = a; g_b = b; g_called = 1;
    }
};
static CQuestMgr g_obj;
static CQuestMgr* g_questMgr = &g_obj;

void __stdcall CGameScriptInterface_ActivateQuest(int quest)
{
    g_questMgr->Activate(quest, 1, 1);
}

int main()
{
    CGameScriptInterface_ActivateQuest(0x42);
    if (g_called && g_quest == 0x42 && g_a == 1 && g_b == 1) {
        std::printf("CGameScriptInterface_00892e80_TEST PASS\n");
        return 0;
    }
    std::printf("CGameScriptInterface_00892e80_TEST FAIL\n");
    return 1;
}