// CGameScriptInterface::TellHeroQuestObjectiveCompleted
struct CQuestManager
{
    void SetObjectiveAsCompleted(unsigned long objectiveId);
};

extern CQuestManager* DAT_013b89fc;

void __fastcall CGameScriptInterface_TellHeroQuestObjectiveCompleted(
    void*,
    unsigned long,
    unsigned long objectiveId)
{
    DAT_013b89fc->SetObjectiveAsCompleted(objectiveId);
}
