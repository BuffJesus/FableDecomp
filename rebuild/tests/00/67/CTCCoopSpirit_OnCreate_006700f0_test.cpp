#include <stdio.h>
#include <string.h>

struct Definition { int references; int destroyed; };
struct Thing { unsigned char pad[0x8c]; unsigned short definitionIndex; };
struct Spirit {
    unsigned long vftable;
    Thing* thing;
    unsigned long pad08;
    long noMovementTimer;
    long score;
    long offscreenCounter;
    long movement[3];
    void* master;
    Definition* coopSpiritDefinition;
    void* particleEmitter[2];
};

static void ModelOnCreate(Spirit* spirit, Definition* resolved, Definition* subdefinition)
{
    spirit->particleEmitter[1] = 0;
    spirit->movement[2] = 0;
    spirit->movement[1] = 0;
    spirit->movement[0] = 0;
    spirit->noMovementTimer = 0;
    spirit->master = 0;
    spirit->offscreenCounter = 0;
    spirit->score = 0;
    spirit->coopSpiritDefinition = subdefinition;
    if (resolved != 0 && --resolved->references == 0)
        resolved->destroyed = 1;
}

int main()
{
    Spirit spirit;
    Thing thing;
    Definition retained = { 2, 0 };
    Definition released = { 1, 0 };
    Definition sub = { 7, 0 };
    memset(&spirit, 0x5a, sizeof(spirit));
    memset(&thing, 0, sizeof(thing));
    thing.definitionIndex = 0x2345;
    spirit.thing = &thing;
    ModelOnCreate(&spirit, &retained, &sub);
    if (thing.definitionIndex != 0x2345 || spirit.noMovementTimer != 0 ||
        spirit.score != 0 || spirit.offscreenCounter != 0 || spirit.master != 0 ||
        spirit.movement[0] != 0 || spirit.movement[1] != 0 || spirit.movement[2] != 0 ||
        spirit.particleEmitter[1] != 0 || spirit.coopSpiritDefinition != &sub ||
        retained.references != 1 || retained.destroyed != 0)
        return 1;
    ModelOnCreate(&spirit, &released, 0);
    if (released.references != 0 || released.destroyed != 1 ||
        spirit.coopSpiritDefinition != 0)
        return 2;
    puts("COOP_SPIRIT_ONCREATE_PASS");
    return 0;
}