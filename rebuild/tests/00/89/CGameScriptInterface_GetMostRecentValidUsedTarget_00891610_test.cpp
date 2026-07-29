#include <cstdio>

class CBaseObject;
class CScriptThing;
class CThing;
class CVillageTaskCarryCrate;

namespace NHeroInformationScreens
{
class CBase
{
public:
    CBase();
};
}

extern const unsigned char
    CGSIGetMostRecentValidUsedTarget_EmptyVTable = 0;
static const unsigned char g_ValidVTable = 0;

class CScriptThing : public NHeroInformationScreens::CBase
{
public:
    CScriptThing()
        : m_VTable(
              &CGSIGetMostRecentValidUsedTarget_EmptyVTable),
          m_Field04(0),
          m_Field08(0)
    {
    }

    const void* volatile m_VTable;
    volatile unsigned long m_Field04;
    volatile unsigned long m_Field08;
};

class CBaseObject
{
public:
    CScriptThing BuildScriptThing();

    unsigned char m_Unused[0x91];
    unsigned char m_Flags;
};

class CBaseIntelligentPointer
{
public:
    CBaseObject* GetPItem();

    CBaseObject* m_Item;
};

class CThing
{
};

class CVillageTaskCarryCrate
{
public:
    void AssignCreatureToTask(CThing* thing);

    unsigned char m_Unused[0xA8];
    CBaseIntelligentPointer m_MostRecentTarget;
};

class CGameScriptInterface
{
public:
    virtual CScriptThing
        GetMostRecentValidUsedTarget() const;
};

CVillageTaskCarryCrate*
    CGSIGetMostRecentValidUsedTarget_Manager;
static CVillageTaskCarryCrate* g_ManagerAfterGet;
static CBaseIntelligentPointer*
    g_ObservedIntelligentPointer;
static CVillageTaskCarryCrate* g_ObservedAssignManager;
static CThing* g_ObservedAssignedThing;
static CBaseObject* g_ObservedBuildObject;
static void* g_ObservedBaseThis;
static unsigned long g_GetPItemCallCount;
static unsigned long g_AssignCallCount;
static unsigned long g_BuildCallCount;
static unsigned long g_BaseConstructorCallCount;

NHeroInformationScreens::CBase::CBase()
{
    g_ObservedBaseThis = this;
    ++g_BaseConstructorCallCount;
}

CBaseObject* CBaseIntelligentPointer::GetPItem()
{
    g_ObservedIntelligentPointer = this;
    ++g_GetPItemCallCount;
    if (g_ManagerAfterGet != 0)
        CGSIGetMostRecentValidUsedTarget_Manager =
            g_ManagerAfterGet;
    return m_Item;
}

void CVillageTaskCarryCrate::AssignCreatureToTask(
    CThing* thing)
{
    g_ObservedAssignManager = this;
    g_ObservedAssignedThing = thing;
    ++g_AssignCallCount;
}

CScriptThing CBaseObject::BuildScriptThing()
{
    g_ObservedBuildObject = this;
    ++g_BuildCallCount;
    CScriptThing result;
    result.m_VTable = &g_ValidVTable;
    result.m_Field04 = 0x11223344;
    result.m_Field08 = 0x55667788;
    return result;
}

static void ResetObservations()
{
    g_ManagerAfterGet = 0;
    g_ObservedIntelligentPointer = 0;
    g_ObservedAssignManager = 0;
    g_ObservedAssignedThing =
        reinterpret_cast<CThing*>(1);
    g_ObservedBuildObject = 0;
    g_ObservedBaseThis = 0;
    g_GetPItemCallCount = 0;
    g_AssignCallCount = 0;
    g_BuildCallCount = 0;
    g_BaseConstructorCallCount = 0;
}

static int CheckEmptyResult(
    const CScriptThing& result)
{
    if (result.m_VTable !=
            &CGSIGetMostRecentValidUsedTarget_EmptyVTable ||
        result.m_Field04 != 0 ||
        result.m_Field08 != 0)
        return 1;
    return 0;
}

int main()
{
    CVillageTaskCarryCrate firstManager = {};
    CVillageTaskCarryCrate secondManager = {};
    CBaseObject item = {};
    CGameScriptInterface interfaceObject;

    ResetObservations();
    firstManager.m_MostRecentTarget.m_Item = 0;
    CGSIGetMostRecentValidUsedTarget_Manager =
        &firstManager;
    const CScriptThing nullResult =
        interfaceObject.GetMostRecentValidUsedTarget();
    if (CheckEmptyResult(nullResult) != 0 ||
        g_ObservedIntelligentPointer !=
            &firstManager.m_MostRecentTarget ||
        g_GetPItemCallCount != 1 ||
        g_AssignCallCount != 0 ||
        g_BuildCallCount != 0 ||
        g_BaseConstructorCallCount != 1 ||
        g_ObservedBaseThis != &nullResult)
        return 1;

    ResetObservations();
    item.m_Flags = 1;
    firstManager.m_MostRecentTarget.m_Item = &item;
    CGSIGetMostRecentValidUsedTarget_Manager =
        &firstManager;
    const CScriptThing flaggedResult =
        interfaceObject.GetMostRecentValidUsedTarget();
    if (CheckEmptyResult(flaggedResult) != 0 ||
        g_ObservedIntelligentPointer !=
            &firstManager.m_MostRecentTarget ||
        g_GetPItemCallCount != 1 ||
        g_AssignCallCount != 0 ||
        g_BuildCallCount != 0 ||
        g_BaseConstructorCallCount != 1 ||
        g_ObservedBaseThis != &flaggedResult)
        return 2;

    ResetObservations();
    item.m_Flags = 0;
    firstManager.m_MostRecentTarget.m_Item = &item;
    g_ManagerAfterGet = &secondManager;
    CGSIGetMostRecentValidUsedTarget_Manager =
        &firstManager;
    const CScriptThing validResult =
        interfaceObject.GetMostRecentValidUsedTarget();
    if (validResult.m_VTable != &g_ValidVTable ||
        validResult.m_Field04 != 0x11223344 ||
        validResult.m_Field08 != 0x55667788 ||
        g_ObservedIntelligentPointer !=
            &firstManager.m_MostRecentTarget ||
        g_ObservedAssignManager != &secondManager ||
        g_ObservedAssignedThing != 0 ||
        g_ObservedBuildObject != &item ||
        g_GetPItemCallCount != 1 ||
        g_AssignCallCount != 1 ||
        g_BuildCallCount != 1 ||
        g_BaseConstructorCallCount != 1)
        return 3;

    std::printf("FSE2_00891610_TEST PASS\n");
    return 0;
}
