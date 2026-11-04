#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_game_event_dispatch.h"
#include <stdio.h>
#include <string.h>

void CWorld::ReceiveLocalEvent(const CGameEvent*) {}

const CGameEventDispatch CMainGameComponent::EventDispatch[87] = {
    {&CWorld::ReceiveLocalEvent, 0, 0, 0},
    {0, 0, 0, 0},
    {&CWorld::ReceiveLocalEvent, 0, 0, 0},
    {0, 0, 0, 0}
};

int main()
{
    unsigned char storage[sizeof(CWorld)];
    memset(storage, 0xa5, sizeof(storage));
    CWorld* world = reinterpret_cast<CWorld*>(storage);
    for (int i = 0; i < 16; ++i) {
        CGameEvent event(i);
        const bool expected = i == 0 || i == 2;
        if (world->CWorld::EventIsWorldEvent(&event) != expected) return 1;
        for (unsigned j = 0; j < sizeof(storage); ++j)
            if (storage[j] != 0xa5) return 2;
    }
    puts("WORLD_EVENT_IS_WORLD_PASS cases=16");
    return 0;
}
