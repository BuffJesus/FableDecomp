void CGameScriptInterface::GetAllActiveQuestInfo(
    std::vector<CCharString>& outQuestNames,
    std::vector<CCharString>& outQuestInfo) const
{
    auto* const questManager = *reinterpret_cast<CQuestManager* const*>(0x013B89FC);
    questManager->GetAllActiveQuestInfo(outQuestNames, outQuestInfo);
}