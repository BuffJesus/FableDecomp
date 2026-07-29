class CCharString;

class CQuestManager
{
public:
    void RemoveActiveQuestCard(
        const CCharString& questCardScriptName);
};

class CGameScriptInterface
{
public:
    virtual void RemoveQuestCardFromHero(
        const CCharString& questCardScriptName) const;
};

extern CQuestManager* CGSIRemoveQuestCardFromHero_Manager;

void CGameScriptInterface::RemoveQuestCardFromHero(
    const CCharString& questCardScriptName) const
{
    CGSIRemoveQuestCardFromHero_Manager->RemoveActiveQuestCard(
        questCardScriptName);
}
