class CCharString;

class CQuestManager
{
public:
    bool IsQuestActive(const CCharString& questName);
};

class CGameScriptInterface
{
public:
    virtual bool IsQuestActive(
        const CCharString& questName) const;
};

extern CQuestManager* CGSIIsQuestActive_Manager;

bool CGameScriptInterface::IsQuestActive(
    const CCharString& questName) const
{
    return CGSIIsQuestActive_Manager->IsQuestActive(questName);
}
