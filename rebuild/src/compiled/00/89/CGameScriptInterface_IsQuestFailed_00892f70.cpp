class CCharString;

class CQuestManager
{
public:
    bool IsQuestFailed(const CCharString& questName);
};

class CGameScriptInterface
{
public:
    virtual bool IsQuestFailed(
        const CCharString& questName) const;
};

extern CQuestManager* CGSIIsQuestFailed_Manager;

bool CGameScriptInterface::IsQuestFailed(
    const CCharString& questName) const
{
    return CGSIIsQuestFailed_Manager->IsQuestFailed(questName);
}
