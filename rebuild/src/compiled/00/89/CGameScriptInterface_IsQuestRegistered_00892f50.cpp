class CCharString;

class CQuestManager
{
public:
    bool IsQuestRegistered(const CCharString& questName);
};

class CGameScriptInterface
{
public:
    virtual bool IsQuestRegistered(const CCharString& questName) const;
};

extern CQuestManager* CGSIIsQuestRegistered_Manager;

bool CGameScriptInterface::IsQuestRegistered(
    const CCharString& questName) const
{
    return CGSIIsQuestRegistered_Manager->IsQuestRegistered(questName);
}
