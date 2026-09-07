#include <stdio.h>

struct Vec3 { unsigned long x, y, z; };
struct Thing { Vec3 position; signed char heroSlot; long fieldB0, fieldB4; };
struct Trace {
    int step;
    int swapStep, createStep, addStep, destroyStep, resolveStep, positionStep;
    int slot;
    Vec3 forwarded;
};

static void ModelSwapToHero(Thing* thing, Trace* trace)
{
    Vec3 saved = thing->position;
    trace->slot = (int)thing->heroSlot;
    trace->swapStep = ++trace->step;
    trace->createStep = ++trace->step;
    trace->addStep = ++trace->step;
    trace->destroyStep = ++trace->step;
    trace->resolveStep = ++trace->step;
    trace->forwarded = saved;
    trace->positionStep = ++trace->step;
    thing->fieldB4 = thing->fieldB0;
}

int main()
{
    Thing thing = {{0x11223344UL, 0x55667788UL, 0x99aabbccUL}, -3,
                   0x12345678L, 0};
    Trace trace = {};
    ModelSwapToHero(&thing, &trace);
    if (trace.slot != -3 || trace.swapStep != 1 || trace.createStep != 2 ||
        trace.addStep != 3 || trace.destroyStep != 4 || trace.resolveStep != 5 ||
        trace.positionStep != 6 || trace.forwarded.x != 0x11223344UL ||
        trace.forwarded.y != 0x55667788UL || trace.forwarded.z != 0x99aabbccUL ||
        thing.fieldB4 != thing.fieldB0)
        return 1;
    puts("COOP_SPIRIT_SWAP_TO_HERO_PASS");
    return 0;
}