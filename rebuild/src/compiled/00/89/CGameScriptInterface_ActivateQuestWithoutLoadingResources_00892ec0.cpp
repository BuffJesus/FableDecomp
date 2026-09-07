#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString;

struct CQuestManager {
  void Activate(CCharString* cs, int a, int b);
};

extern CQuestManager* g_questMgr;

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void ActivateQuestWithoutLoadingResources(CCharString* cs);
};

void CGameScriptInterface_Methods::ActivateQuestWithoutLoadingResources(CCharString* cs)
{
  g_questMgr->Activate(cs, 1, 0);
}