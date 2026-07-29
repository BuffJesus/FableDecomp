#include <cstdio>

#include "../../../src/compiled/00/59/CFrontEndManager_GotoSpecificPreviousScreen_0059686d.cpp"

#define FE_PREVIOUS_DEFINE_COMPONENT_SLOT(Name) \
    void CComponent_0059686d::Name() {}
FE_PREVIOUS_SLOT_BLOCK_0(FE_PREVIOUS_DEFINE_COMPONENT_SLOT)
FE_PREVIOUS_SLOT_BLOCK_1(FE_PREVIOUS_DEFINE_COMPONENT_SLOT)
FE_PREVIOUS_SLOT_BLOCK_2(FE_PREVIOUS_DEFINE_COMPONENT_SLOT)
#undef FE_PREVIOUS_DEFINE_COMPONENT_SLOT

void CComponent_0059686d::SetCurrentState(int)
{
}

void CManager_0059686d::Slot00() {}
void CManager_0059686d::Slot01() {}
void CManager_0059686d::Slot02() {}
void CManager_0059686d::Slot03() {}
void CManager_0059686d::Slot04() {}
void CManager_0059686d::ReleaseComponentOwnership(void*) {}

static int g_ProfileGetCalls;
static int g_ProfileLoadCalls;
static int g_DistanceCalls;
static int g_BackCalls;
static int g_PopCalls;
static int g_PushCalls;
static int g_OwnershipCalls;
static void* g_ReleasedOwnership;
static CUserProfileManager_0059686d g_Profile;

class TestManager_0059686d : public CManager_0059686d
{
public:
    virtual void ReleaseComponentOwnership(void* ownership)
    {
        ++g_OwnershipCalls;
        g_ReleasedOwnership = ownership;
    }
};

class TestComponent_0059686d : public CComponent_0059686d
{
public:
    int identifier;
    int stateCalls;
    int lastState;

    void Reset(int value)
    {
        identifier = value;
        stateCalls = 0;
        lastState = -1;
        ownership = (void*)(0x1000 + value * 0x10);
    }

    virtual void SetCurrentState(int state)
    {
        ++stateCalls;
        lastState = state;
    }
};

static TestManager_0059686d g_Manager;
static CFrontEndManager_0059686d g_FrontEnd;
static TestComponent_0059686d g_Components[5];
static CComponent_0059686d* g_HistoryStorage[8];

extern "C" CUserProfileManager_0059686d* __cdecl
FrontEndUserProfileGet_0059686d()
{
    ++g_ProfileGetCalls;
    return &g_Profile;
}

extern "C" void __fastcall FrontEndUserProfileLoad_0059686d(
    CUserProfileManager_0059686d* profile,
    void*)
{
    if (profile == &g_Profile)
        ++g_ProfileLoadCalls;
}

extern "C" long __fastcall FrontEndDequeDistance_0059686d(
    FrontEndDequeIterator_0059686d* finish,
    void*,
    const FrontEndDequeIterator_0059686d* start)
{
    ++g_DistanceCalls;
    return (long)(finish->current - start->current);
}

extern "C" CComponent_0059686d** __fastcall FrontEndDequeBack_0059686d(
    FrontEndComponentDeque_0059686d* history,
    void*)
{
    ++g_BackCalls;
    return history->end.current - 1;
}

extern "C" void __fastcall FrontEndDequePopBack_0059686d(
    FrontEndComponentDeque_0059686d* history,
    void*)
{
    ++g_PopCalls;
    --history->end.current;
}

extern "C" void __fastcall FrontEndDequePushBack_0059686d(
    FrontEndComponentDeque_0059686d* history,
    void*,
    CComponent_0059686d* const* component)
{
    ++g_PushCalls;
    *history->end.current = *component;
    ++history->end.current;
}

extern "C" CManager_0059686d* __cdecl FrontEndManagerGet_0059686d()
{
    return &g_Manager;
}

static void ResetFixture(
    int historyCount,
    CComponent_0059686d* first,
    CComponent_0059686d* second,
    CComponent_0059686d* third)
{
    for (int index = 0; index < 5; ++index)
        g_Components[index].Reset(index);

    g_HistoryStorage[0] = first;
    g_HistoryStorage[1] = second;
    g_HistoryStorage[2] = third;

    g_FrontEnd.previousScreens.begin.current = g_HistoryStorage;
    g_FrontEnd.previousScreens.begin.first = g_HistoryStorage;
    g_FrontEnd.previousScreens.begin.last = g_HistoryStorage + 8;
    g_FrontEnd.previousScreens.begin.node = 0;
    g_FrontEnd.previousScreens.end.current =
        g_HistoryStorage + historyCount;
    g_FrontEnd.previousScreens.end.first = g_HistoryStorage;
    g_FrontEnd.previousScreens.end.last = g_HistoryStorage + 8;
    g_FrontEnd.previousScreens.end.node = 0;

    g_FrontEnd.transitionFrom = (CComponent_0059686d*)0x11111111;
    g_FrontEnd.transitionTo = (CComponent_0059686d*)0x22222222;

    g_ProfileGetCalls = 0;
    g_ProfileLoadCalls = 0;
    g_DistanceCalls = 0;
    g_BackCalls = 0;
    g_PopCalls = 0;
    g_PushCalls = 0;
    g_OwnershipCalls = 0;
    g_ReleasedOwnership = 0;
}

static int CheckCommonTransition(
    CComponent_0059686d* expectedPrevious,
    CComponent_0059686d* expectedTarget)
{
    int failures = 0;
    if (g_ProfileGetCalls != 1 || g_ProfileLoadCalls != 1)
    {
        std::printf("profile load contract failed\n");
        ++failures;
    }
    if (g_FrontEnd.transitionFrom != expectedPrevious ||
        g_FrontEnd.transitionTo != expectedTarget)
    {
        std::printf("transition field contract failed\n");
        ++failures;
    }
    if (g_OwnershipCalls != 1 ||
        g_ReleasedOwnership !=
            &((CComponent_0059686d*)expectedPrevious)->ownership)
    {
        std::printf("component ownership contract failed\n");
        ++failures;
    }
    return failures;
}

static int CheckRecoveredLayout()
{
    if ((char*)&g_Components[0].ownership -
            (char*)(CComponent_0059686d*)&g_Components[0] != 0x04 ||
        (char*)&g_FrontEnd.previousScreens - (char*)&g_FrontEnd != 0x20 ||
        (char*)&g_FrontEnd.transitionFrom - (char*)&g_FrontEnd != 0x98 ||
        (char*)&g_FrontEnd.transitionTo - (char*)&g_FrontEnd != 0x9c)
    {
        std::printf("recovered layout contract failed\n");
        return 1;
    }
    return 0;
}

int main()
{
    int failures = CheckRecoveredLayout();

    // Empty and single-entry histories only load the profile.  They must not
    // mutate transitions, states, ownership, or deque contents.
    ResetFixture(0, 0, 0, 0);
    g_FrontEnd.GotoSpecificPreviousScreen(&g_Components[0]);
    if (g_ProfileGetCalls != 1 ||
        g_ProfileLoadCalls != 1 ||
        g_DistanceCalls != 1 ||
        g_BackCalls != 0 ||
        g_PopCalls != 0 ||
        g_PushCalls != 0 ||
        g_OwnershipCalls != 0 ||
        g_FrontEnd.transitionFrom !=
            (CComponent_0059686d*)0x11111111)
    {
        std::printf("empty-history guard failed\n");
        ++failures;
    }

    ResetFixture(1, &g_Components[0], 0, 0);
    g_FrontEnd.GotoSpecificPreviousScreen(&g_Components[0]);
    if (g_DistanceCalls != 1 ||
        g_BackCalls != 0 ||
        g_PopCalls != 0 ||
        g_OwnershipCalls != 0 ||
        g_Components[0].stateCalls != 0)
    {
        std::printf("single-history guard failed\n");
        ++failures;
    }

    // Search through the stack until the requested screen is found.
    ResetFixture(
        3,
        &g_Components[0],
        &g_Components[1],
        &g_Components[2]);
    g_FrontEnd.GotoSpecificPreviousScreen(&g_Components[0]);
    failures += CheckCommonTransition(
        &g_Components[2],
        &g_Components[0]);
    if (g_Components[2].stateCalls != 1 ||
        g_Components[2].lastState != 6 ||
        g_PopCalls != 2 ||
        g_BackCalls != 3 ||
        g_PushCalls != 0 ||
        g_FrontEnd.previousScreens.end.current != g_HistoryStorage + 1 ||
        g_HistoryStorage[0] != &g_Components[0])
    {
        std::printf("existing-screen search/pop failed\n");
        ++failures;
    }

    // If the requested screen is absent, retail drains the history and pushes
    // the requested pointer back as the new sole entry.
    ResetFixture(
        3,
        &g_Components[0],
        &g_Components[1],
        &g_Components[2]);
    g_FrontEnd.GotoSpecificPreviousScreen(&g_Components[4]);
    failures += CheckCommonTransition(
        &g_Components[2],
        &g_Components[4]);
    if (g_PopCalls != 3 ||
        g_BackCalls != 3 ||
        g_PushCalls != 1 ||
        g_FrontEnd.previousScreens.end.current != g_HistoryStorage + 1 ||
        g_HistoryStorage[0] != &g_Components[4])
    {
        std::printf("missing-screen restore failed\n");
        ++failures;
    }

    // An immediately previous target leaves all older history intact.
    ResetFixture(
        3,
        &g_Components[0],
        &g_Components[1],
        &g_Components[2]);
    g_FrontEnd.GotoSpecificPreviousScreen(&g_Components[1]);
    failures += CheckCommonTransition(
        &g_Components[2],
        &g_Components[1]);
    if (g_PopCalls != 1 ||
        g_BackCalls != 2 ||
        g_PushCalls != 0 ||
        g_FrontEnd.previousScreens.end.current != g_HistoryStorage + 2)
    {
        std::printf("immediate-previous transition failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059686d_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059686d_TEST PASS\n");
    return 0;
}
