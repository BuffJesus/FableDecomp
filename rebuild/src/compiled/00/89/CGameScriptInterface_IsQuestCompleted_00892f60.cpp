class CCharString;

class CQuestManager
{
public:
    bool IsQuestCompleted(const CCharString& questName);
};

class CGameScriptInterface
{
public:
    virtual bool IsQuestCompleted(const CCharString& questName) const;
};

extern CQuestManager* CGSIIsQuestCompleted_Manager;

bool CGameScriptInterface::IsQuestCompleted(
    const CCharString& questName) const
{
    return CGSIIsQuestCompleted_Manager->IsQuestCompleted(questName);
}
