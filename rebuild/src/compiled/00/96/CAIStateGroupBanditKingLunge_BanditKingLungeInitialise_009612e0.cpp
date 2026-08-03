struct CAIBrain;
struct CThingAICreature;
struct CCharString;

typedef long EScriptAIPriority;

struct CAIStateGroup_BanditKingLunge {
    unsigned int vtbl;
    char pad[0x18];
    long TimeOfLastLunge;
};

extern "C" void __fastcall CAIStateGroupBase_Initialise(CAIStateGroup_BanditKingLunge* self, void* edx, CAIBrain& brain, CThingAICreature& creature, long param3, EScriptAIPriority priority, CCharString const& name);

extern "C" void __fastcall CAIStateGroup_BanditKingLunge_Initialise(CAIStateGroup_BanditKingLunge* self, void* edx, CAIBrain& brain, CThingAICreature& creature, long param3, EScriptAIPriority priority, CCharString const& name)
{
    self->TimeOfLastLunge = 0;
    CAIStateGroupBase_Initialise(self, edx, brain, creature, param3, priority, name);
}