#include "engine/CGameScriptInterface.h"

struct CQuestMgr { void doFailQuest(int a1,char a2,int a3,int a4); void notify(int a1); };
extern CQuestMgr* g_questMgr; extern char g_someFlag;
struct CGameScriptInterface_Methods : CGameScriptInterface { void applyFloat(float v); };
void __fastcall CGameScriptInterface_SetQuestAsFailed(CGameScriptInterface* self,int,int a1,char a2,int a3,int a4)
{
 g_questMgr->doFailQuest(a1,a2,a3,a4);
 if(!a2&&g_someFlag)g_questMgr->notify(a1);
 ((CGameScriptInterface_Methods*)self)->applyFloat(1.0f);
}
