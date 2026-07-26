class CQuestManager;

extern CQuestManager* DAT_013B89FC;

class CGameScriptInterface
{
public:
    virtual void TellHeroQuestObjectiveCompleted(unsigned long objectiveId) const;
};

class CQuestManager
{
public:
    void SetObjectiveAsCompleted(unsigned long objectiveId);
};

void CGameScriptInterface::TellHeroQuestObjectiveCompleted(unsigned long objectiveId) const
{
    DAT_013B89FC->SetObjectiveAsCompleted(objectiveId);
}