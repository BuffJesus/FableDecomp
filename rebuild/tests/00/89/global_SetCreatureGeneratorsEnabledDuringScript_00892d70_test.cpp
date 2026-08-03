#include <cstdio>

class CCharString
{
public:
    int tag;
};

class CQuestManagerFake;

static int g_calls = 0;
static CQuestManagerFake* g_lastThis = 0;
static const CCharString* g_lastName = 0;
static bool g_lastEnabled = false;

class CQuestManagerFake
{
public:
    void __fastcall SetCreatureGeneratorState(const CCharString& generatorName, bool enabled)
    {
        g_calls++;
        g_lastThis = this;
        g_lastName = &generatorName;
        g_lastEnabled = enabled;
    }
};

typedef CQuestManagerFake CQuestManager;

CQuestManager* g_QuestManager = 0;

class CGameScriptInterface
{
public:
    void __fastcall SetCreatureGeneratorsEnabledDuringScript(const CCharString& generatorName, bool enabled) const;
};

void __fastcall CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript(const CCharString& generatorName, bool enabled) const
{
    g_QuestManager->SetCreatureGeneratorState(generatorName, enabled);
}

int main()
{
    CQuestManagerFake qm;
    g_QuestManager = &qm;

    CCharString name;
    name.tag = 1234;

    CGameScriptInterface iface;
    iface.SetCreatureGeneratorsEnabledDuringScript(name, true);

    bool ok = (g_calls == 1) && (g_lastThis == &qm) && (g_lastName == &name) && (g_lastEnabled == true);
    if (ok)
        std::printf("SETCREATUREGEN_OK\n");
    else
        std::printf("SETCREATUREGEN_FAIL calls=%d\n", g_calls);
    return 0;
}