#include <cstdio>

static int g_notifyCount = 0;
static int g_reloadCount = 0;
static float g_lastProgress = 0.0f;
static int g_lastNotifyA0 = 0;

struct CQuestMgr {
    void Notify(int a0, char a1, int a2, int a3);
    void Reload(int a0);
};

void CQuestMgr::Notify(int a0, char a1, int a2, int a3) {
    (void)a1; (void)a2; (void)a3;
    g_notifyCount++;
    g_lastNotifyA0 = a0;
}
void CQuestMgr::Reload(int a0) {
    (void)a0;
    g_reloadCount++;
}

CQuestMgr g_mgrObj;
CQuestMgr* g_questMgr = &g_mgrObj;
char g_someFlag = 1;

struct CGameScriptInterface {
    void SetQuestProgress(float v);
};
void CGameScriptInterface::SetQuestProgress(float v) {
    g_lastProgress = v;
}

void __fastcall CGameScriptInterface_SetQuestAsCompleted(CGameScriptInterface* self, int edx_ignore, int a0, char a1, int a2, int a3)
{
    g_questMgr->Notify(a0, a1, a2, a3);
    if (!a1 && g_someFlag) {
        g_questMgr->Reload(a0);
    }
    self->SetQuestProgress(1.0f);
}

int main() {
    CGameScriptInterface obj;

    // Case 1: a1 != 0 -> Notify only, no Reload
    g_notifyCount = g_reloadCount = 0; g_lastProgress = 0.0f;
    CGameScriptInterface_SetQuestAsCompleted(&obj, 0, 111, 1, 222, 333);
    if (g_notifyCount != 1) { std::printf("FAIL notify1\n"); return 1; }
    if (g_lastNotifyA0 != 111) { std::printf("FAIL a0_1\n"); return 1; }
    if (g_reloadCount != 0) { std::printf("FAIL reload1\n"); return 1; }
    if (g_lastProgress != 1.0f) { std::printf("FAIL prog1\n"); return 1; }

    // Case 2: a1 == 0, flag set -> Notify + Reload
    g_notifyCount = g_reloadCount = 0; g_lastProgress = 0.0f;
    CGameScriptInterface_SetQuestAsCompleted(&obj, 0, 444, 0, 555, 666);
    if (g_notifyCount != 1) { std::printf("FAIL notify2\n"); return 1; }
    if (g_reloadCount != 1) { std::printf("FAIL reload2\n"); return 1; }
    if (g_lastNotifyA0 != 444) { std::printf("FAIL a0_2\n"); return 1; }
    if (g_lastProgress != 1.0f) { std::printf("FAIL prog2\n"); return 1; }

    // Case 3: a1 == 0, flag clear -> Notify only, no Reload
    g_someFlag = 0;
    g_notifyCount = g_reloadCount = 0; g_lastProgress = 0.0f;
    CGameScriptInterface_SetQuestAsCompleted(&obj, 0, 777, 0, 0, 0);
    if (g_reloadCount != 0) { std::printf("FAIL reload3\n"); return 1; }
    if (g_lastProgress != 1.0f) { std::printf("FAIL prog3\n"); return 1; }

    std::printf("CGameScriptInterface_00892f80_TEST PASS\n");
    return 0;
}