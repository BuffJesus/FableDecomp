class CCharString;
class CQuestManager;

extern CQuestManager* DAT_013B89FC;

class CGameScriptInterface
{
public:
    void GiveHeroNewQuestObjective(const CCharString& string, unsigned long objective_id) const;
};

class CQuestManager
{
public:
    void AddNewObjective(const CCharString& string, unsigned long objective_id);
};

void CGameScriptInterface::GiveHeroNewQuestObjective(const CCharString& string, unsigned long objective_id) const
{
    DAT_013B89FC->AddNewObjective(string, objective_id);
}