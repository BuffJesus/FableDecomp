void CGameScriptInterface::RemoveBoast(long questId, const CCharString& boast)
{
    [[maybe_unused]] CCharString boastCopy(boast);
    DAT_013b89fc->RemoveBoast(questId);
}