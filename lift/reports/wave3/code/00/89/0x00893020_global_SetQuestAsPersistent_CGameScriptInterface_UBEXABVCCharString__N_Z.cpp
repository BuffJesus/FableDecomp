void CGameScriptInterface::SetQuestAsPersistent(const CCharString& questName, bool persistent) const
{
    auto* const questManager = *reinterpret_cast<CQuestManager**>(0x013B89FC);
    questManager->SetQuestAsPersistent(questName, persistent);
}