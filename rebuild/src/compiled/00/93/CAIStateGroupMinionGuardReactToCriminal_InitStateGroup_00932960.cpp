struct CWorld;
struct CThingAICreature;

struct CAIStateGroup_MinionGuardReactToCriminal {
    char pad[0x28];
    unsigned int f28;   // +0x28
    unsigned int f2c;   // +0x2c
    unsigned int f30;   // +0x30
    void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_MinionGuardReactToCriminal::InitStateGroup(
        const CWorld& world, CThingAICreature& creature)
{
    this->f28 = 0u;
    this->f2c = 0xe0000000u;
    this->f30 = 0xffffffffu;
}