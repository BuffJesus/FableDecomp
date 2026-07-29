// CGameScriptInterface::TellHeroQuestObjectiveFailed
struct CQuestManager
{
    void SetObjectiveAsFailed(unsigned long objectiveId);
};

extern CQuestManager* DAT_013b89fc;

void __fastcall CGameScriptInterface_TellHeroQuestObjectiveFailed(
    void*,
    unsigned long,
    unsigned long objectiveId)
{
    DAT_013b89fc->SetObjectiveAsFailed(objectiveId);
}
