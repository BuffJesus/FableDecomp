// CGameScriptInterface::GetAllActiveQuestInfo
struct CCharStringVector;

struct CQuestManager
{
    void GetAllActiveQuestInfo(
        CCharStringVector& questNames,
        CCharStringVector& questInfo);
};

extern CQuestManager* DAT_013b89fc;

void __fastcall CGameScriptInterface_GetAllActiveQuestInfo(
    void*,
    unsigned long,
    CCharStringVector& questNames,
    CCharStringVector& questInfo)
{
    DAT_013b89fc->GetAllActiveQuestInfo(questNames, questInfo);
}
