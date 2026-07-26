struct CGameScriptInterface;

// The quest/story manager reached via a global singleton pointer.
struct CQuestMgr {
    void Notify(int a0, char a1, int a2, int a3);
    void Reload(int a0);
};

extern CQuestMgr* g_questMgr;   // ds:0x13b89fc
extern char g_someFlag;         // ds:0x1375741

struct CGameScriptInterface {
    void SetQuestProgress(float v);
};

// __fastcall member modeled as free __fastcall: this in ecx, dummy absorbs edx,
// the four real args (a0,a1,a2,a3) are pushed on the stack.
void __fastcall CGameScriptInterface_SetQuestAsCompleted(CGameScriptInterface* self, int edx_ignore, int a0, char a1, int a2, int a3)
{
    g_questMgr->Notify(a0, a1, a2, a3);
    if (!a1 && g_someFlag) {
        g_questMgr->Reload(a0);
    }
    self->SetQuestProgress(1.0f);
}