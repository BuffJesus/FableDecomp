#include <cstdio>

class CStopWatch;

namespace NHeroInformationScreens
{
class CBase
{
public:
    CBase();
    virtual ~CBase();
};
}

class CScriptThing : public NHeroInformationScreens::CBase
{
public:
    CScriptThing()
        : m_Field04(0),
          m_Field08(0)
    {
    }

    virtual ~CScriptThing();

    unsigned long GetField04() const
    {
        return m_Field04;
    }

    unsigned long GetField08() const
    {
        return m_Field08;
    }

    void SetFields(unsigned long field04, unsigned long field08)
    {
        m_Field04 = field04;
        m_Field08 = field08;
    }

private:
    unsigned long m_Field04;
    unsigned long m_Field08;
};

class CStopWatch
{
public:
    void* GetTicks(unsigned __int64 uid) const;
};

class CGSIGetThingWithUID_Dispatch
{
public:
    virtual void Reserved00();
    virtual void Reserved04();
    virtual void Reserved08();
    virtual CStopWatch* GetStopWatch();
};

struct CGSIGetThingWithUID_InterfaceOverlay
{
    unsigned char m_Pad00[0x04];
    CGSIGetThingWithUID_Dispatch* m_pDispatch;
};

class CGameScriptInterface
{
public:
    virtual CScriptThing GetThingWithUID(unsigned __int64 uid) const;
};

class CGSIGetThingWithUID_Thing
{
public:
    CScriptThing WrapForScript() const;
};

static unsigned long g_BaseConstructorCalls;
static unsigned long g_DispatchCalls;
static unsigned long g_LookupCalls;
static unsigned long g_WrapCalls;
static void* g_ExpectedDispatchThis;
static unsigned __int64 g_ExpectedUID;
static void* g_LookupResult;
static void* g_ExpectedThing;

NHeroInformationScreens::CBase::CBase()
{
    ++g_BaseConstructorCalls;
}

NHeroInformationScreens::CBase::~CBase()
{
}

CScriptThing::~CScriptThing()
{
}

void CGSIGetThingWithUID_Dispatch::Reserved00()
{
}

void CGSIGetThingWithUID_Dispatch::Reserved04()
{
}

void CGSIGetThingWithUID_Dispatch::Reserved08()
{
}

CStopWatch* CGSIGetThingWithUID_Dispatch::GetStopWatch()
{
    if (this != g_ExpectedDispatchThis)
    {
        std::printf("dispatch this mismatch\n");
        return 0;
    }

    ++g_DispatchCalls;
    return reinterpret_cast<CStopWatch*>(0x12345678);
}

void* CStopWatch::GetTicks(unsigned __int64 uid) const
{
    if (this != reinterpret_cast<const CStopWatch*>(0x12345678))
    {
        std::printf("lookup this mismatch\n");
        return 0;
    }
    if (uid != g_ExpectedUID)
    {
        std::printf("UID mismatch\n");
        return 0;
    }

    ++g_LookupCalls;
    return g_LookupResult;
}

CScriptThing CGSIGetThingWithUID_Thing::WrapForScript() const
{
    CScriptThing result;
    ++g_WrapCalls;
    if (this != g_ExpectedThing)
    {
        std::printf("thing mismatch\n");
        return result;
    }

    result.SetFields(0xA1B2C3D4, 0xE5F60718);
    return result;
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_00891d50_TEST FAIL: %s\n", pMessage);
    return 1;
}

int main()
{
    CGSIGetThingWithUID_Dispatch dispatch;
    CGSIGetThingWithUID_InterfaceOverlay interfaceStorage = {};
    CGameScriptInterface& interfaceObject =
        reinterpret_cast<CGameScriptInterface&>(interfaceStorage);

    interfaceStorage.m_pDispatch = &dispatch;
    g_ExpectedDispatchThis = &dispatch;

    g_ExpectedUID = 0xFEDCBA9876543210ui64;
    g_ExpectedThing = reinterpret_cast<void*>(0x0BADF00D);
    g_LookupResult = g_ExpectedThing;
    CScriptThing found =
        interfaceObject.CGameScriptInterface::GetThingWithUID(g_ExpectedUID);

    if (g_DispatchCalls != 1 || g_LookupCalls != 1 || g_WrapCalls != 1)
        return Fail("success call counts");
    if (found.GetField04() != 0xA1B2C3D4 ||
        found.GetField08() != 0xE5F60718)
        return Fail("success result");

    const unsigned long baseCallsBeforeMiss = g_BaseConstructorCalls;
    g_ExpectedUID = 0x0123456789ABCDEFui64;
    g_LookupResult = 0;
    CScriptThing missing =
        interfaceObject.CGameScriptInterface::GetThingWithUID(g_ExpectedUID);

    if (g_DispatchCalls != 2 || g_LookupCalls != 2 || g_WrapCalls != 1)
        return Fail("failure call counts");
    if (g_BaseConstructorCalls != baseCallsBeforeMiss + 1)
        return Fail("failure construction");
    if (missing.GetField04() != 0 || missing.GetField08() != 0)
        return Fail("failure result");

    std::printf("FSE2_00891d50_TEST PASS\n");
    return 0;
}
