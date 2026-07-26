#include <cstdio>

struct CQuestMgr {
    int failCount;
    int notifyCount;
    int lastArg1;
    void doFailQuest(int a1, char a2, int a3, int a4){ failCount++; lastArg1=a1; (void)a2;(void)a3;(void)a4; }
    void notify(int a1){ notifyCount++; (void)a1; }
};

CQuestMgr gm = {0,0,0};
CQuestMgr* g_questMgr = &gm;
char g_someFlag = 1;

struct CGameScriptInterface {
    float applied;
    void applyFloat(float v){ applied=v; }
};

void CGameScriptInterface_SetQuestAsFailed(CGameScriptInterface* self,
                                           int arg1, char arg2, int arg3, int arg4)
{
    g_questMgr->doFailQuest(arg1, arg2, arg3, arg4);
    if (!arg2 && g_someFlag) {
        g_questMgr->notify(arg1);
    }
    self->applyFloat(1.0f);
}

int main(){
    CGameScriptInterface gsi; gsi.applied=0.0f;
    // arg2==0 and g_someFlag==1 -> notify called
    CGameScriptInterface_SetQuestAsFailed(&gsi, 7, 0, 0, 0);
    if (gm.failCount!=1 || gm.notifyCount!=1 || gm.lastArg1!=7 || gsi.applied!=1.0f) { std::printf("FAIL branch A\n"); return 1; }
    // arg2!=0 -> no notify
    CGameScriptInterface_SetQuestAsFailed(&gsi, 9, 1, 0, 0);
    if (gm.failCount!=2 || gm.notifyCount!=1) { std::printf("FAIL branch B\n"); return 1; }
    std::printf("CGameScriptInterface_00892fd0_TEST PASS\n");
    return 0;
}