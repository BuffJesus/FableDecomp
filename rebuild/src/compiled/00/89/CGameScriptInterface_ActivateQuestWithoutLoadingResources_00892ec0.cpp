struct CCharString;

struct CQuestManager {
  void Activate(CCharString* cs, int a, int b);
};

extern CQuestManager* g_questMgr;

struct CGameScriptInterface {
  void ActivateQuestWithoutLoadingResources(CCharString* cs);
};

void CGameScriptInterface::ActivateQuestWithoutLoadingResources(CCharString* cs)
{
  g_questMgr->Activate(cs, 1, 0);
}