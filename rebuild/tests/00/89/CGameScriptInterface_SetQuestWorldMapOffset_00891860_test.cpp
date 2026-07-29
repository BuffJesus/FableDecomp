#include <cstdio>

class CCharString
{
public:
    explicit CCharString(unsigned long identity)
        : m_Identity(identity)
    {
    }

    unsigned long m_Identity;
};

class C2DCoordI
{
public:
    C2DCoordI(long x, long y)
        : m_X(x),
          m_Y(y)
    {
    }

    long m_X;
    long m_Y;
};

class CQuestManager
{
public:
    void SetQuestWorldMapOffset(
        const CCharString& questName,
        const C2DCoordI& offset);
};

class CGameScriptInterface
{
public:
    virtual void SetQuestWorldMapOffset(
        const CCharString& questName,
        const C2DCoordI& offset) const;
};

CQuestManager* CGSISetQuestWorldMapOffset_Manager;
static CQuestManager* g_ObservedManager;
static const CCharString* g_ObservedQuestName;
static const C2DCoordI* g_ObservedOffset;
static unsigned long g_CallCount;

void CQuestManager::SetQuestWorldMapOffset(
    const CCharString& questName,
    const C2DCoordI& offset)
{
    g_ObservedManager = this;
    g_ObservedQuestName = &questName;
    g_ObservedOffset = &offset;
    ++g_CallCount;
}

int main()
{
    CQuestManager manager;
    CGameScriptInterface interfaceObject;
    CCharString firstQuest(0x11223344);
    CCharString secondQuest(0x55667788);
    C2DCoordI firstOffset(-123, 456);
    C2DCoordI secondOffset(789, -1011);
    CGSISetQuestWorldMapOffset_Manager = &manager;

    interfaceObject.SetQuestWorldMapOffset(firstQuest, firstOffset);
    if (g_CallCount != 1 || g_ObservedManager != &manager ||
        g_ObservedQuestName != &firstQuest ||
        g_ObservedOffset != &firstOffset)
        return 1;

    interfaceObject.SetQuestWorldMapOffset(secondQuest, secondOffset);
    if (g_CallCount != 2 || g_ObservedManager != &manager ||
        g_ObservedQuestName != &secondQuest ||
        g_ObservedOffset != &secondOffset)
        return 2;

    std::printf("FSE2_00891860_TEST PASS\n");
    return 0;
}
