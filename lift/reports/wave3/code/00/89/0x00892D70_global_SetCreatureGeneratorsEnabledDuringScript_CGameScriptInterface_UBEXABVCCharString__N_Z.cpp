class CCharString;
class CQuestManager;

class CGameScriptInterface
{
public:
    virtual void __thiscall SetCreatureGeneratorsEnabledDuringScript(
        const CCharString& generatorName,
        bool enabled) const;
};

class CQuestManager
{
public:
    void __thiscall SetCreatureGeneratorState(
        const CCharString& generatorName,
        bool enabled);
};

inline constexpr auto g_QuestManager =
    reinterpret_cast<CQuestManager**>(0x013B89FC);

void __thiscall CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript(
    const CCharString& generatorName,
    bool enabled) const
{
    (*g_QuestManager)->SetCreatureGeneratorState(generatorName, enabled);
}