#include <cstdio>

class CGSISheatheHeroWeapons_Target;

class CCreatureActionBase
{
public:
    explicit CCreatureActionBase(unsigned long identity)
        : m_Identity(identity)
    {
    }

    unsigned long m_Identity;
};

class CGSISheatheHeroWeapons_LookupResult
{
public:
    CGSISheatheHeroWeapons_Target* ResolveTarget();
};

class CGSISheatheHeroWeapons_LookupSource
{
public:
    CGSISheatheHeroWeapons_LookupResult* BeginLookup();
};

class CGSISheatheHeroWeapons_Target
{
public:
    bool IsThingCarryingWeapon();
    void PrepareSheathe();
    void UpdateSheathe();
    void FinaliseSheathe();
    void SetCurrentAction(CCreatureActionBase* action);

    unsigned char m_Unused[0x91];
    unsigned char m_Flags;
};

class CGSISheatheHeroWeapons_ActionStorage
{
public:
    CCreatureActionBase* Construct(
        CGSISheatheHeroWeapons_Target* target,
        CCreatureActionBase* previousAction,
        int priority);
    void Destroy();

    unsigned char m_Bytes[0xB0];
};

class CGameScriptInterface
{
public:
    virtual void SheatheHeroWeapons() const;

    unsigned char m_Unused[0x10];
    CGSISheatheHeroWeapons_LookupSource* m_LookupSource;
};

static CGSISheatheHeroWeapons_LookupSource*
    g_ObservedLookupSource;
static CGSISheatheHeroWeapons_LookupResult*
    g_LookupResult;
static CGSISheatheHeroWeapons_LookupResult*
    g_ObservedLookupResult;
static CGSISheatheHeroWeapons_Target* g_Target;
static CGSISheatheHeroWeapons_Target*
    g_ObservedTarget;
static bool g_IsCarryingWeapon;
static CGSISheatheHeroWeapons_ActionStorage*
    g_ObservedConstructStorage;
static CGSISheatheHeroWeapons_Target*
    g_ObservedConstructTarget;
static CCreatureActionBase*
    g_ObservedPreviousAction;
static int g_ObservedPriority;
static CCreatureActionBase* g_Action;
static CGSISheatheHeroWeapons_ActionStorage*
    g_ObservedDestroyStorage;
static CCreatureActionBase* g_ObservedCurrentAction;
static unsigned long g_Events[10];
static unsigned long g_EventCount;

static void RecordEvent(unsigned long event)
{
    if (g_EventCount < 10)
        g_Events[g_EventCount] = event;
    ++g_EventCount;
}

CGSISheatheHeroWeapons_LookupResult*
CGSISheatheHeroWeapons_LookupSource::BeginLookup()
{
    g_ObservedLookupSource = this;
    RecordEvent(1);
    return g_LookupResult;
}

CGSISheatheHeroWeapons_Target*
CGSISheatheHeroWeapons_LookupResult::ResolveTarget()
{
    g_ObservedLookupResult = this;
    RecordEvent(2);
    return g_Target;
}

bool CGSISheatheHeroWeapons_Target::IsThingCarryingWeapon()
{
    g_ObservedTarget = this;
    RecordEvent(3);
    return g_IsCarryingWeapon;
}

void CGSISheatheHeroWeapons_Target::PrepareSheathe()
{
    g_ObservedTarget = this;
    RecordEvent(4);
}

void CGSISheatheHeroWeapons_Target::UpdateSheathe()
{
    g_ObservedTarget = this;
    RecordEvent(5);
}

void CGSISheatheHeroWeapons_Target::FinaliseSheathe()
{
    g_ObservedTarget = this;
    RecordEvent(6);
}

CCreatureActionBase*
CGSISheatheHeroWeapons_ActionStorage::Construct(
    CGSISheatheHeroWeapons_Target* target,
    CCreatureActionBase* previousAction,
    int priority)
{
    g_ObservedConstructStorage = this;
    g_ObservedConstructTarget = target;
    g_ObservedPreviousAction = previousAction;
    g_ObservedPriority = priority;
    RecordEvent(7);
    return g_Action;
}

void CGSISheatheHeroWeapons_Target::SetCurrentAction(
    CCreatureActionBase* action)
{
    g_ObservedTarget = this;
    g_ObservedCurrentAction = action;
    RecordEvent(8);
}

void CGSISheatheHeroWeapons_ActionStorage::Destroy()
{
    g_ObservedDestroyStorage = this;
    RecordEvent(9);
}

static void ResetObservations()
{
    g_ObservedLookupSource = 0;
    g_ObservedLookupResult = 0;
    g_ObservedTarget = 0;
    g_ObservedConstructStorage = 0;
    g_ObservedConstructTarget = 0;
    g_ObservedPreviousAction =
        reinterpret_cast<CCreatureActionBase*>(1);
    g_ObservedPriority = 0;
    g_ObservedDestroyStorage = 0;
    g_ObservedCurrentAction = 0;
    g_EventCount = 0;
}

static bool CheckEvents(
    const unsigned long* expected,
    unsigned long count)
{
    if (g_EventCount != count)
        return false;
    for (unsigned long index = 0;
         index < count;
         ++index)
    {
        if (g_Events[index] != expected[index])
            return false;
    }
    return true;
}

int main()
{
    CGSISheatheHeroWeapons_LookupSource lookupSource;
    CGSISheatheHeroWeapons_LookupResult lookupResult;
    CGSISheatheHeroWeapons_Target target = {};
    CCreatureActionBase action(0x11223344);
    CGameScriptInterface interfaceObject;
    interfaceObject.m_LookupSource = &lookupSource;
    g_LookupResult = &lookupResult;
    g_Action = &action;

    const unsigned long lookupEvents[] = {1, 2};
    ResetObservations();
    g_Target = 0;
    interfaceObject.SheatheHeroWeapons();
    if (!CheckEvents(lookupEvents, 2) ||
        g_ObservedLookupSource != &lookupSource ||
        g_ObservedLookupResult != &lookupResult)
        return 1;

    ResetObservations();
    g_Target = &target;
    target.m_Flags = 1;
    interfaceObject.SheatheHeroWeapons();
    if (!CheckEvents(lookupEvents, 2) ||
        g_ObservedTarget != 0)
        return 2;

    const unsigned long unarmedEvents[] = {1, 2, 3};
    ResetObservations();
    target.m_Flags = 0;
    g_IsCarryingWeapon = false;
    interfaceObject.SheatheHeroWeapons();
    if (!CheckEvents(unarmedEvents, 3) ||
        g_ObservedTarget != &target)
        return 3;

    const unsigned long sheatheEvents[] = {
        1, 2, 3, 4, 5, 5, 6, 7, 8, 9
    };
    ResetObservations();
    target.m_Flags = 2;
    g_IsCarryingWeapon = true;
    interfaceObject.SheatheHeroWeapons();
    if (!CheckEvents(sheatheEvents, 10) ||
        g_ObservedTarget != &target ||
        g_ObservedConstructTarget != &target ||
        g_ObservedPreviousAction != 0 ||
        g_ObservedPriority != 100 ||
        g_ObservedCurrentAction != &action ||
        g_ObservedConstructStorage == 0 ||
        g_ObservedDestroyStorage !=
            g_ObservedConstructStorage)
        return 4;

    std::printf("FSE2_008916a0_TEST PASS\n");
    return 0;
}
