extern CQuestManager* DAT_013b89fc;

bool CGameScriptInterface::IsQuestCompleted(const CCharString& questName) const
{
    return DAT_013b89fc->IsQuestCompleted(questName);
}