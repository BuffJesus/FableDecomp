#include <cstdio>

class CScriptThing
{
public:
    explicit CScriptThing(unsigned long identity)
        : m_Identity(identity)
    {
    }

    unsigned long m_Identity;
};

class CGameScriptInterface
{
public:
    virtual bool CanThingBe_Smelled_ByOtherThing(
        const CScriptThing& thing,
        const CScriptThing& otherThing) const;

private:
    bool CanThingBe_Sensed_ByOtherThing(
        const CScriptThing& thing,
        const CScriptThing& otherThing,
        long senseType) const;
};

static const CGameScriptInterface* g_ObservedInterface;
static const CScriptThing* g_ObservedThing;
static const CScriptThing* g_ObservedOtherThing;
static long g_ObservedSenseType;
static bool g_Result;
static unsigned long g_CallCount;

bool CGameScriptInterface::CanThingBe_Sensed_ByOtherThing(
    const CScriptThing& thing,
    const CScriptThing& otherThing,
    long senseType) const
{
    g_ObservedInterface = this;
    g_ObservedThing = &thing;
    g_ObservedOtherThing = &otherThing;
    g_ObservedSenseType = senseType;
    ++g_CallCount;
    return g_Result;
}

int main()
{
    CGameScriptInterface interfaceObject;
    CScriptThing firstThing(0x11223344);
    CScriptThing secondThing(0x55667788);

    g_Result = false;
    if (interfaceObject.CanThingBe_Smelled_ByOtherThing(
            firstThing,
            secondThing))
        return 1;
    if (g_CallCount != 1 ||
        g_ObservedInterface != &interfaceObject ||
        g_ObservedThing != &firstThing ||
        g_ObservedOtherThing != &secondThing ||
        g_ObservedSenseType != 3)
        return 2;

    g_Result = true;
    if (!interfaceObject.CanThingBe_Smelled_ByOtherThing(
            secondThing,
            firstThing))
        return 3;
    if (g_CallCount != 2 ||
        g_ObservedInterface != &interfaceObject ||
        g_ObservedThing != &secondThing ||
        g_ObservedOtherThing != &firstThing ||
        g_ObservedSenseType != 3)
        return 4;

    std::printf("FSE2_0089cd50_TEST PASS\n");
    return 0;
}
