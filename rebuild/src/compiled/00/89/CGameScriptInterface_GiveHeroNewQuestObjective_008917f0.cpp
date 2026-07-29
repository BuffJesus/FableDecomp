class CCharString;

class CQuestManager
{
public:
    void AddNewObjective(
        const CCharString& questScriptName,
        unsigned long objectiveId);
};

class CGameScriptInterface
{
public:
    virtual void GiveHeroNewQuestObjective(
        const CCharString& questScriptName,
        unsigned long objectiveId) const;
};

extern CQuestManager* CGSIGiveHeroNewQuestObjective_Manager;

void CGameScriptInterface::GiveHeroNewQuestObjective(
    const CCharString& questScriptName,
    unsigned long objectiveId) const
{
    CGSIGiveHeroNewQuestObjective_Manager->AddNewObjective(
        questScriptName,
        objectiveId);
}
