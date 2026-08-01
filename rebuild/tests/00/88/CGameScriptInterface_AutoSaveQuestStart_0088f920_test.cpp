#include <cstdio>

static int g_called = 0;
static void* g_lastThis = 0;

class CWorld
{
public:
    void AutoSaveQuestStart();
};

void CWorld::AutoSaveQuestStart()
{
    g_called = 1;
    g_lastThis = this;
}

class CGameScriptInterface
{
public:
    void* m_pVtbl;
    CWorld* m_pWorld;
    void AutoSaveQuestStart() const;
};

void CGameScriptInterface::AutoSaveQuestStart() const
{
    m_pWorld->AutoSaveQuestStart();
}

int main()
{
    CWorld world;
    CGameScriptInterface gsi;
    gsi.m_pVtbl = 0;
    gsi.m_pWorld = &world;
    gsi.AutoSaveQuestStart();
    if (g_called == 1 && g_lastThis == (void*)&world)
    {
        printf("AUTOSAVEQUESTSTART_OK\n");
    }
    return 0;
}