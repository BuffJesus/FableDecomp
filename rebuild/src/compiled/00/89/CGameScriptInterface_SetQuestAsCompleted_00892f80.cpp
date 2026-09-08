#include "engine/CGameScriptInterface.h"

struct CQuestMgr { void Notify(int a0, char a1, int a2, int a3); void Reload(int a0); };
extern CQuestMgr* g_questMgr;
extern char g_someFlag;
struct CGameScriptInterface_Methods : CGameScriptInterface { void SetQuestProgress(float v); };

void __fastcall CGameScriptInterface_SetQuestAsCompleted(CGameScriptInterface* self, int, int a0, char a1, int a2, int a3)
{
    g_questMgr->Notify(a0, a1, a2, a3);
    if (!a1 && g_someFlag) g_questMgr->Reload(a0);
    ((CGameScriptInterface_Methods*)self)->SetQuestProgress(1.0f);
}
