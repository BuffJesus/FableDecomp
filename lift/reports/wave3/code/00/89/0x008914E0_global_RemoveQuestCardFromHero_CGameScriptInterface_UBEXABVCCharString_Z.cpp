class CCharString;
class CQuestManager;

class CGameScriptInterface
{
public:
    void __thiscall RemoveQuestCardFromHero(const CCharString& questCardScriptName) const;
};

class CQuestManager
{
public:
    void __thiscall RemoveActiveQuestCard(const CCharString& questCardScriptName);
};

extern CQuestManager* DAT_013b89fc;

void __thiscall CGameScriptInterface::RemoveQuestCardFromHero(
    const CCharString& questCardScriptName) const
{
    DAT_013b89fc->RemoveActiveQuestCard(questCardScriptName);
}