class CCharString;
class CQuestManager;

extern CQuestManager* DAT_013b89fc;

class CGameScriptInterface
{
public:
    virtual void DeactivateQuest(const CCharString& quest_name, unsigned long flags) const;
};

class CQuestManager
{
public:
    void DeactivateQuest(const CCharString& quest_name, unsigned long flags);
};

void CGameScriptInterface::DeactivateQuest(const CCharString& quest_name, unsigned long flags) const
{
    DAT_013b89fc->DeactivateQuest(quest_name, flags);
}