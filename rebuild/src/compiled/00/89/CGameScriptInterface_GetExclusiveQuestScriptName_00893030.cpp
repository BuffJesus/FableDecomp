class CCharString;

class CQuestManager
{
public:
    const CCharString& GetActiveExclusiveQuestName();
};

class CGameScriptInterface
{
public:
    virtual const CCharString& GetExclusiveQuestScriptName() const;
};

extern CQuestManager* CGSIGetExclusiveQuestScriptName_Manager;

const CCharString&
CGameScriptInterface::GetExclusiveQuestScriptName() const
{
    return CGSIGetExclusiveQuestScriptName_Manager
        ->GetActiveExclusiveQuestName();
}
