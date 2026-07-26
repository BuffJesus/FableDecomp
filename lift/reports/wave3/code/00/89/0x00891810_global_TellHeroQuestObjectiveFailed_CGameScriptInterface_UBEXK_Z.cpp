class CQuestManager;

extern CQuestManager* DAT_013B89FC;

class CGameScriptInterface
{
public:
    virtual void TellHeroQuestObjectiveFailed(unsigned long objectiveId) const;
};

class CQuestManager
{
public:
    void SetObjectiveAsFailed(unsigned long objectiveId);
};

void CGameScriptInterface::TellHeroQuestObjectiveFailed(unsigned long objectiveId) const
{
    DAT_013B89FC->SetObjectiveAsFailed(objectiveId);
}
