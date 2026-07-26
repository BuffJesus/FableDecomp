// CGameScriptInterface::SetQuestAsFailed @ 00892fd0
struct CQuestMgr {
    void doFailQuest(int a1, char a2, int a3, int a4);
    void notify(int a1);
};

extern CQuestMgr* g_questMgr;      // ds:0x13b89fc
extern char g_someFlag;            // ds:0x1375741

struct CGameScriptInterface {
    void applyFloat(float v);
};

void __fastcall CGameScriptInterface_SetQuestAsFailed(CGameScriptInterface* self, int /*edx*/,
                                                      int arg1, char arg2, int arg3, int arg4)
{
    g_questMgr->doFailQuest(arg1, arg2, arg3, arg4);
    if (!arg2 && g_someFlag) {
        g_questMgr->notify(arg1);
    }
    self->applyFloat(1.0f);
}