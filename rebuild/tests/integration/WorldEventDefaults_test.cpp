#define FABLE_WORLD_EVENT_REAL_DEFAULTS
#define main DispatchTableFixtureMain
#include "DispatchTable_test.cpp"
#undef main
#include <stdio.h>
#include <string.h>

int main() {
    unsigned scenarios=0;
    for (unsigned pattern=0;pattern<256;++pattern) {
        unsigned long storage[104];memset(storage,pattern,sizeof(storage));
        CWorld* world=reinterpret_cast<CWorld*>(&storage[2]);
        unsigned long before[104];memcpy(before,storage,sizeof(storage));
        unsigned long eventWords[12];memset(eventWords,255-pattern,sizeof(eventWords));
        const CGameEvent* event=reinterpret_cast<const CGameEvent*>(&eventWords[1]);
        unsigned long eventBefore[12];memcpy(eventBefore,eventWords,sizeof(eventWords));
        world->EAApplyAllScriptedMapBrushes(event);++scenarios;
        if (memcmp(before,storage,sizeof(storage)) || memcmp(eventBefore,eventWords,sizeof(eventWords))) return 1;
        world->LEASetEditorMode(event);++scenarios;
        if (memcmp(before,storage,sizeof(storage)) || memcmp(eventBefore,eventWords,sizeof(eventWords))) return 2;
    }
    if (DispatchTableFixtureMain()) return 3;
    printf("WORLD_EVENT_DEFAULTS_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
