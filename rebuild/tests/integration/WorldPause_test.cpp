#define FABLE_WORLD_EVENT_REAL_PAUSE
#define main WorldEventControlFixtureMain
#include "WorldEventControl_test.cpp"
#undef main

struct WorldPauseAccess : CWorld { using CWorld::SetAsPaused; };
int main() {
    unsigned scenarios=0;
    for (unsigned pattern=0;pattern<256;++pattern) for(unsigned value=0;value<2;++value) {
        unsigned long storage[104];memset(storage,pattern,sizeof(storage));
        WorldPauseAccess* world=reinterpret_cast<WorldPauseAccess*>(&storage[2]);
        unsigned char expected[sizeof(storage)];memcpy(expected,storage,sizeof(storage));expected[8+0xa4]=value;
        world->SetAsPaused(value!=0);++scenarios;
        if(memcmp(expected,storage,sizeof(storage))) return 1;
        world->SetAsPaused(value!=0);++scenarios;
        if(memcmp(expected,storage,sizeof(storage))) return 2;
    }
    if(WorldEventControlFixtureMain()) return 3;
    printf("WORLD_PAUSE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
