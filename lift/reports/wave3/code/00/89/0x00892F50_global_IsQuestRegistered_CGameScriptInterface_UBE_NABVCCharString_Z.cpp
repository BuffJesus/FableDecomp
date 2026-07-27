class CCharString;
class CQuestManager;

extern CQuestManager* DAT_013B89FC;

bool CGameScriptInterface::IsQuestRegistered(const CCharString& questName) const
{
    return DAT_013B89FC->IsQuestRegistered(questName);
}