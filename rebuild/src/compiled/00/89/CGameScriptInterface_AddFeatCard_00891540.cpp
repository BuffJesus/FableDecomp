class CCharString;

class CQuestManager
{
public:
    void AddFeatCard(
        long featCardId,
        const CCharString& title,
        const CCharString& description);
};

class CGameScriptInterface
{
public:
    virtual void AddFeatCard(
        long featCardId,
        const CCharString& title,
        const CCharString& description) const;
};

extern CQuestManager* CGSIAddFeatCard_Manager;

void CGameScriptInterface::AddFeatCard(
    long featCardId,
    const CCharString& title,
    const CCharString& description) const
{
    CGSIAddFeatCard_Manager->AddFeatCard(
        featCardId,
        title,
        description);
}
