extern CQuestManager* DAT_013b89fc;

void CGameScriptInterface::SetQuestWorldMapOffset(
    const CCharString& questName,
    const C2DCoordI& offset
) const
{
    CQuestManager::SetQuestWorldMapOffset(DAT_013b89fc, &questName, &offset);
}