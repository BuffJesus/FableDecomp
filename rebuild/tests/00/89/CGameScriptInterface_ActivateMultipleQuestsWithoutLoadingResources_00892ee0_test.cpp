#include <cstdio>
struct CCharStringArray { int n; };

static CCharStringArray const* g_lastQuests;
static int g_lastA, g_lastB, g_called;

struct CQuestMgr {
    void Activate(CCharStringArray const& quests, int a, int b);
};
void CQuestMgr::Activate(CCharStringArray const& quests, int a, int b) {
    g_lastQuests = &quests; g_lastA = a; g_lastB = b; g_called = 1;
}
CQuestMgr realMgr;
CQuestMgr* g_questMgr = &realMgr;

void __stdcall CGameScriptInterface_ActivateMultipleQuestsWithoutLoadingResources(CCharStringArray const& quests);

int main() {
    CCharStringArray arr; arr.n = 7;
    CGameScriptInterface_ActivateMultipleQuestsWithoutLoadingResources(arr);
    if (g_called == 1 && g_lastQuests == &arr && g_lastA == 1 && g_lastB == 0) {
        printf("OK_00892ee0\n");
    } else {
        printf("FAIL called=%d a=%d b=%d\n", g_called, g_lastA, g_lastB);
    }
    return 0;
}