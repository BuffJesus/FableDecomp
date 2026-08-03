#include <cstdio>

struct CAIBrain { int x; };
struct CThingAICreature { int x; };
struct CCharString { int x; };

typedef long EScriptAIPriority;

struct CAIStateGroup_BanditKingLunge {
    unsigned int vtbl;
    char pad[0x18];
    long TimeOfLastLunge;
};

static int g_called = 0;
static CAIStateGroup_BanditKingLunge* g_self_seen = 0;
static CAIBrain* g_brain_seen = 0;
static CThingAICreature* g_creature_seen = 0;
static long g_param3_seen = -1;
static EScriptAIPriority g_priority_seen = -1;
static CCharString* g_name_seen = 0;

extern "C" void __fastcall CAIStateGroupBase_Initialise(CAIStateGroup_BanditKingLunge* self, void* edx, CAIBrain& brain, CThingAICreature& creature, long param3, EScriptAIPriority priority, CCharString const& name)
{
    g_called = 1;
    g_self_seen = self;
    g_brain_seen = &brain;
    g_creature_seen = &creature;
    g_param3_seen = param3;
    g_priority_seen = priority;
    g_name_seen = (CCharString*)&name;
}

extern "C" void __fastcall CAIStateGroup_BanditKingLunge_Initialise(CAIStateGroup_BanditKingLunge* self, void* edx, CAIBrain& brain, CThingAICreature& creature, long param3, EScriptAIPriority priority, CCharString const& name)
{
    self->TimeOfLastLunge = 0;
    CAIStateGroupBase_Initialise(self, 0, brain, creature, param3, priority, name);
}

int main()
{
    CAIStateGroup_BanditKingLunge obj;
    obj.TimeOfLastLunge = 12345;

    CAIBrain brain; brain.x = 1;
    CThingAICreature creature; creature.x = 2;
    CCharString nm; nm.x = 3;

    CAIStateGroup_BanditKingLunge_Initialise(&obj, 0, brain, creature, 7, 2, nm);

    if (obj.TimeOfLastLunge != 0) { printf("FAIL field not zeroed\n"); return 1; }
    if (!g_called) { printf("FAIL base not called\n"); return 1; }
    if (g_self_seen != &obj) { printf("FAIL wrong self\n"); return 1; }
    if (g_brain_seen != &brain) { printf("FAIL wrong brain\n"); return 1; }
    if (g_creature_seen != &creature) { printf("FAIL wrong creature\n"); return 1; }
    if (g_param3_seen != 7) { printf("FAIL wrong param3\n"); return 1; }
    if (g_priority_seen != 2) { printf("FAIL wrong priority\n"); return 1; }
    if (g_name_seen != (CCharString*)&nm) { printf("FAIL wrong name ptr\n"); return 1; }

    printf("BANDITKINGLUNGE_INIT_OK\n");
    return 0;
}