#include <cstdio>
#include <cstring>

class CCharString
{
public:
    CCharString(const char* text, long length);
    ~CCharString();

    const char* m_Text;
};

class CGameScriptInterface
{
public:
    virtual void AddQuestCard(
        const CCharString& questName,
        const CCharString& questDescription,
        bool addToFront,
        bool suppressNotification) const;

    void AddQuestCardEntry(
        const CCharString& questName,
        const CCharString& questDescription,
        bool addToFront);
};

class CGSIAddQuestCard_Hooks
{
public:
    int GetValue() const;
    void Notify(
        const CCharString& message,
        const CCharString& orbName,
        bool immediate,
        bool unknown) const;
};

union CGSIAddQuestCard_GetValuePointer
{
    int (CGSIAddQuestCard_Hooks::*method)() const;
    unsigned long raw;
};

union CGSIAddQuestCard_NotifyPointer
{
    void (CGSIAddQuestCard_Hooks::*method)(
        const CCharString&,
        const CCharString&,
        bool,
        bool) const;
    unsigned long raw;
};

struct CGSIAddQuestCard_TestVTable
{
    unsigned long m_Slots[(0xA34 / 4) + 1];
};

long CGSIAddQuestCard_CurrentValue;
CGameScriptInterface* CGSIAddQuestCard_Manager;

static const CGameScriptInterface* g_ObservedHookThis;
static long g_GetValues[2];
static unsigned long g_GetValueCallCount;
static const CCharString* g_Constructed[2];
static const char* g_ConstructedText[2];
static long g_ConstructedLength[2];
static unsigned long g_ConstructorCallCount;
static const CCharString* g_Destructed[2];
static unsigned long g_DestructorCallCount;
static const CCharString* g_ObservedMessage;
static const CCharString* g_ObservedOrbName;
static bool g_ObservedImmediate;
static bool g_ObservedUnknown;
static unsigned long g_NotifyCallCount;
static CGameScriptInterface* g_ObservedManager;
static const CCharString* g_ObservedQuestName;
static const CCharString* g_ObservedQuestDescription;
static bool g_ObservedAddToFront;
static unsigned long g_AddEntryCallCount;

CCharString::CCharString(
    const char* text,
    long length)
    : m_Text(text)
{
    if (g_ConstructorCallCount < 2)
    {
        g_Constructed[g_ConstructorCallCount] = this;
        g_ConstructedText[g_ConstructorCallCount] = text;
        g_ConstructedLength[g_ConstructorCallCount] = length;
    }
    ++g_ConstructorCallCount;
}

CCharString::~CCharString()
{
    if (g_DestructorCallCount < 2)
        g_Destructed[g_DestructorCallCount] = this;
    ++g_DestructorCallCount;
}

int CGSIAddQuestCard_Hooks::GetValue() const
{
    g_ObservedHookThis =
        reinterpret_cast<const CGameScriptInterface*>(this);
    const unsigned long index = g_GetValueCallCount;
    ++g_GetValueCallCount;
    return g_GetValues[index < 2 ? index : 1];
}

void CGSIAddQuestCard_Hooks::Notify(
    const CCharString& message,
    const CCharString& orbName,
    bool immediate,
    bool unknown) const
{
    g_ObservedHookThis =
        reinterpret_cast<const CGameScriptInterface*>(this);
    g_ObservedMessage = &message;
    g_ObservedOrbName = &orbName;
    g_ObservedImmediate = immediate;
    g_ObservedUnknown = unknown;
    ++g_NotifyCallCount;
}

void CGameScriptInterface::AddQuestCardEntry(
    const CCharString& questName,
    const CCharString& questDescription,
    bool addToFront)
{
    g_ObservedManager = this;
    g_ObservedQuestName = &questName;
    g_ObservedQuestDescription = &questDescription;
    g_ObservedAddToFront = addToFront;
    ++g_AddEntryCallCount;
}

static void ResetObservations()
{
    g_ObservedHookThis = 0;
    g_GetValueCallCount = 0;
    g_Constructed[0] = 0;
    g_Constructed[1] = 0;
    g_ConstructedText[0] = 0;
    g_ConstructedText[1] = 0;
    g_ConstructedLength[0] = 0;
    g_ConstructedLength[1] = 0;
    g_ConstructorCallCount = 0;
    g_Destructed[0] = 0;
    g_Destructed[1] = 0;
    g_DestructorCallCount = 0;
    g_ObservedMessage = 0;
    g_ObservedOrbName = 0;
    g_ObservedImmediate = false;
    g_ObservedUnknown = true;
    g_NotifyCallCount = 0;
    g_ObservedManager = 0;
    g_ObservedQuestName = 0;
    g_ObservedQuestDescription = 0;
    g_ObservedAddToFront = false;
    g_AddEntryCallCount = 0;
}

static int CheckEntry(
    const CGameScriptInterface& interfaceObject,
    CGameScriptInterface& manager,
    const CCharString& questName,
    const CCharString& questDescription,
    bool addToFront)
{
    if (g_ObservedHookThis != &interfaceObject ||
        g_ObservedManager != &manager ||
        g_ObservedQuestName != &questName ||
        g_ObservedQuestDescription != &questDescription ||
        g_ObservedAddToFront != addToFront ||
        g_AddEntryCallCount != 1)
        return 1;
    return 0;
}

int main()
{
    CGSIAddQuestCard_TestVTable testVTable = {};
    CGSIAddQuestCard_GetValuePointer getValuePointer;
    CGSIAddQuestCard_NotifyPointer notifyPointer;
    CGameScriptInterface interfaceObject;
    CGameScriptInterface manager;
    CCharString questName("QUEST_NAME", -1);
    CCharString questDescription("QUEST_DESCRIPTION", -1);
    getValuePointer.method =
        &CGSIAddQuestCard_Hooks::GetValue;
    notifyPointer.method =
        &CGSIAddQuestCard_Hooks::Notify;
    testVTable.m_Slots[0xA34 / 4] =
        getValuePointer.raw;
    testVTable.m_Slots[0x448 / 4] =
        notifyPointer.raw;
    *reinterpret_cast<unsigned long*>(&interfaceObject) =
        reinterpret_cast<unsigned long>(
            &testVTable.m_Slots[0]);
    CGSIAddQuestCard_Manager = &manager;

    ResetObservations();
    CGSIAddQuestCard_CurrentValue = 7;
    g_GetValues[0] = 7;
    interfaceObject.AddQuestCard(
        questName,
        questDescription,
        false,
        false);
    if (g_GetValueCallCount != 1 ||
        g_ConstructorCallCount != 0 ||
        g_NotifyCallCount != 0 ||
        g_DestructorCallCount != 0 ||
        CGSIAddQuestCard_CurrentValue != 7 ||
        CheckEntry(
            interfaceObject,
            manager,
            questName,
            questDescription,
            false) != 0)
        return 1;

    ResetObservations();
    CGSIAddQuestCard_CurrentValue = 7;
    g_GetValues[0] = 8;
    g_GetValues[1] = 9;
    interfaceObject.AddQuestCard(
        questName,
        questDescription,
        false,
        true);
    if (g_GetValueCallCount != 2 ||
        g_ConstructorCallCount != 0 ||
        g_NotifyCallCount != 0 ||
        g_DestructorCallCount != 0 ||
        CGSIAddQuestCard_CurrentValue != 9 ||
        CheckEntry(
            interfaceObject,
            manager,
            questName,
            questDescription,
            false) != 0)
        return 2;

    ResetObservations();
    CGSIAddQuestCard_CurrentValue = 9;
    g_GetValues[0] = 10;
    g_GetValues[1] = 11;
    interfaceObject.AddQuestCard(
        questName,
        questDescription,
        true,
        false);
    if (g_GetValueCallCount != 2 ||
        g_ConstructorCallCount != 2 ||
        g_ConstructedLength[0] != -1 ||
        g_ConstructedLength[1] != -1 ||
        std::strcmp(
            g_ConstructedText[0],
            "HUD_ORB_QUEST_CORE") != 0 ||
        std::strcmp(
            g_ConstructedText[1],
            "TEXT_QST_078_GM_MSG_NEW_QUEST") != 0 ||
        g_NotifyCallCount != 1 ||
        g_ObservedMessage != g_Constructed[1] ||
        g_ObservedOrbName != g_Constructed[0] ||
        !g_ObservedImmediate ||
        g_ObservedUnknown ||
        g_DestructorCallCount != 2 ||
        g_Destructed[0] != g_Constructed[1] ||
        g_Destructed[1] != g_Constructed[0] ||
        CGSIAddQuestCard_CurrentValue != 11 ||
        CheckEntry(
            interfaceObject,
            manager,
            questName,
            questDescription,
            true) != 0)
        return 3;

    std::printf("FSE2_008913f0_TEST PASS\n");
    return 0;
}
