class CCharString;

class CQuestManager
{
public:
    void __fastcall SetCreatureGeneratorState(const CCharString& generatorName, bool enabled);
};

extern CQuestManager* g_QuestManager;

class CGameScriptInterface
{
public:
    void __fastcall SetCreatureGeneratorsEnabledDuringScript(const CCharString& generatorName, bool enabled) const;
};

void __fastcall CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript(const CCharString& generatorName, bool enabled) const
{
    g_QuestManager->SetCreatureGeneratorState(generatorName, enabled);
}