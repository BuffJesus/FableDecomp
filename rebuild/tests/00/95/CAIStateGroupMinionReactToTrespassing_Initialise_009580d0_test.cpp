#include <cstdio>

class CAIBrain {};
class CThingAICreature {};
class CCharString {};

enum EScriptAIPriority
{
    ESCRIPTAIPRIORITY_DUMMY = 0
};

class CAIStateGroupBase
{
public:
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long param3,
                             EScriptAIPriority priority, const CCharString& name);

    unsigned char m_pad[0x1c - sizeof(void*)];
    long m_field_1c;
};

static bool g_baseCalled = false;

void CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long param3,
                                    EScriptAIPriority priority, const CCharString& name)
{
    g_baseCalled = true;
}

class CAIStateGroup_MinionReactToTrespassing : public CAIStateGroupBase
{
public:
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long param3,
                             EScriptAIPriority priority, const CCharString& name);
};

void CAIStateGroup_MinionReactToTrespassing::Initialise(CAIBrain& brain, CThingAICreature& creature,
                                                          long param3, EScriptAIPriority priority,
                                                          const CCharString& name)
{
    m_field_1c = 0;
    CAIStateGroupBase::Initialise(brain, creature, param3, priority, name);
}

int main()
{
    CAIStateGroup_MinionReactToTrespassing obj;
    obj.m_field_1c = 12345;
    g_baseCalled = false;

    CAIBrain brain;
    CThingAICreature creature;
    CCharString name;

    obj.Initialise(brain, creature, 7, ESCRIPTAIPRIORITY_DUMMY, name);

    if (obj.m_field_1c == 0 && g_baseCalled)
    {
        std::printf("MINION_TRESPASS_INIT_OK\n");
        return 0;
    }
    std::printf("MINION_TRESPASS_INIT_FAIL field=%ld base=%d\n", obj.m_field_1c, (int)g_baseCalled);
    return 1;
}