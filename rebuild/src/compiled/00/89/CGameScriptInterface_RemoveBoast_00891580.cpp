// CGameScriptInterface::RemoveBoast
struct CCharString
{
    CCharString(const CCharString& other);
};

struct CQuestManager
{
    void RemoveBoast(long questId, CCharString boast);
};

extern CQuestManager* DAT_013b89fc;

void __fastcall CGameScriptInterface_RemoveBoast(
    void*,
    unsigned long,
    long questId,
    const CCharString& boast)
{
    CQuestManager* questManager = DAT_013b89fc;
    questManager->RemoveBoast(questId, CCharString(boast));
}
