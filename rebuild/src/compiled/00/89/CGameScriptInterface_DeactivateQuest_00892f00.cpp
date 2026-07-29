// CGameScriptInterface::DeactivateQuest
struct CCharString;

struct CQuestManager
{
    void DeactivateQuest(
        const CCharString& questName,
        unsigned long flags);
};

extern CQuestManager* DAT_013b89fc;

void __fastcall CGameScriptInterface_DeactivateQuest(
    void*,
    unsigned long,
    const CCharString& questName,
    unsigned long flags)
{
    DAT_013b89fc->DeactivateQuest(questName, flags);
}
