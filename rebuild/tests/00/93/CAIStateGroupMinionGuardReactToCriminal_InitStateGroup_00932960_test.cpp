#include <cstdio>

struct CWorld;
struct CThingAICreature;

struct CAIStateGroup_MinionGuardReactToCriminal {
    char pad[0x28];
    unsigned int f28;
    unsigned int f2c;
    unsigned int f30;
    void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_MinionGuardReactToCriminal::InitStateGroup(
        const CWorld& world, CThingAICreature& creature)
{
    this->f28 = 0u;
    this->f2c = 0xe0000000u;
    this->f30 = 0xffffffffu;
}

int main() {
    CAIStateGroup_MinionGuardReactToCriminal obj;
    obj.f28 = 0x11111111u;
    obj.f2c = 0x22222222u;
    obj.f30 = 0x33333333u;

    const CWorld* w = 0;
    CThingAICreature* c = 0;
    obj.InitStateGroup(*w, *c);  // pointers unused inside

    if (obj.f28 == 0u &&
        obj.f2c == 0xe0000000u &&
        obj.f30 == 0xffffffffu) {
        std::printf("00932960_TEST PASS\n");
    } else {
        std::printf("FAIL %08x %08x %08x\n", obj.f28, obj.f2c, obj.f30);
    }
    return 0;
}