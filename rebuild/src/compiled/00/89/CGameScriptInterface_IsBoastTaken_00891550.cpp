// CGameScriptInterface::IsBoastTaken
struct CCharString
{
    CCharString(const CCharString& other);
};

struct CQuestManager
{
    bool IsBoastTaken(long questId, CCharString boast);
};

extern CQuestManager* DAT_013b89fc;

bool __fastcall CGameScriptInterface_IsBoastTaken(
    void*,
    unsigned long,
    long questId,
    const CCharString& boast)
{
    CQuestManager* questManager = DAT_013b89fc;
    return questManager->IsBoastTaken(questId, CCharString(boast));
}
