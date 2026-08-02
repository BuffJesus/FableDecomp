#include <cstdio>

struct CAIBrain {};
struct CThingAICreature {};
struct CCharString {};
enum EScriptAIPriority { EScriptAIPriority_Dummy = 0 };

struct CAIStateGroupBase
{
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long param, EScriptAIPriority priority, const CCharString& name);
    long baseField_0x04;
    long baseField_0x08;
    long baseField_0x0c;
    long baseField_0x10;
    long baseField_0x14;
    long baseField_0x18;
    int base_calls;
    long last_param;
};

void CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long param, EScriptAIPriority priority, const CCharString& name)
{
    base_calls++;
    last_param = param;
}

struct CAIStateGroup_BriarRose : CAIStateGroupBase
{
    long field_0x1c;
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long param, EScriptAIPriority priority, const CCharString& name);
};

void CAIStateGroup_BriarRose::Initialise(CAIBrain& brain, CThingAICreature& creature, long param, EScriptAIPriority priority, const CCharString& name)
{
    field_0x1c = 0;
    CAIStateGroupBase::Initialise(brain, creature, param, priority, name);
}

int main()
{
    CAIBrain brain;
    CThingAICreature creature;
    CCharString name;
    CAIStateGroup_BriarRose group;
    group.field_0x1c = 12345;
    group.base_calls = 0;
    group.last_param = 0;

    group.Initialise(brain, creature, 777, EScriptAIPriority_Dummy, name);

    if (group.field_0x1c == 0 && group.base_calls == 1 && group.last_param == 777)
    {
        printf("PASS_MARK_BRIARROSE\n");
    }
    else
    {
        printf("FAIL\n");
    }
    return 0;
}