void CGameScriptInterface::IsBoastTaken(long param_1, const CCharString& param_2) const
{
    CQuestManager* const pQuestManager = DAT_013b89fc;
    CCharString aCStack_8(param_2);
    pQuestManager->IsBoastTaken(param_1);
}