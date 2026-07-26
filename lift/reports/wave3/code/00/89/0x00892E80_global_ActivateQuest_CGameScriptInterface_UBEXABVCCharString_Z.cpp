class CCharString;
class CQuestManager;
class CGameScriptInterface;

class CQuestManager
{
public:
    void __thiscall ActivateQuest(const CCharString& questName, int unkFlag1, int unkFlag2);
};

void __thiscall CGameScriptInterface::ActivateQuest(const CCharString& questName) const
{
    auto* const questManager = *reinterpret_cast<CQuestManager**>(0x013B89FC);
    questManager->ActivateQuest(questName, 1, 1);
}