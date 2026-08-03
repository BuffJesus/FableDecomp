struct CAIBrain;
struct CThingAICreature;
struct CCharString;
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
};

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