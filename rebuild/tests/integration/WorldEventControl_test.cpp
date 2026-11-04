#include "fable_world.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static CWorld* expectedWorld;
static const CGameEvent* expectedEvent;
static unsigned calls, reads, errors;
static bool expectedPause;
void CWorld::UpdateFrame() { if(this!=expectedWorld) ++errors; ++calls; }
void CWorld::HandleMoveHeroToRegionGameEvent(const CGameEvent& event) {
    if(this!=expectedWorld || &event!=expectedEvent) ++errors; ++calls;
}
#ifndef FABLE_WORLD_EVENT_REAL_PAUSE
void CWorld::SetAsPaused(bool value) {
    if(this!=expectedWorld || value!=expectedPause || reads!=1) ++errors; ++calls;
}
#endif
// GetData's general circular-buffer reader remains a dependency. This boundary
// accepts only the exact one-byte reads these four real event handlers request.
void CGameEventDataPackage::GetData(long size, void* dest) {
    if(size!=1 || StartPos!=0 || EndPos!=0 || Length!=expectedEvent->EndPos || Length<1 ||
       memcmp(Data,expectedEvent->Data,Length)) abort();
    *static_cast<unsigned char*>(dest)=Data[0]; --Length; ++StartPos; ++reads;
}
int main() {
    unsigned scenarios=0;
    for (unsigned pattern=0;pattern<256;++pattern) {
        unsigned long storage[104];memset(storage,255-pattern,sizeof(storage));
        CWorld* world=reinterpret_cast<CWorld*>(&storage[2]);expectedWorld=world;
        unsigned long before[104];memcpy(before,storage,sizeof(before));
        CGameEvent event(0x7fffffff);memset(event.Data,pattern,sizeof(event.Data));expectedEvent=&event;
        CGameEvent eventBefore;
        calls=reads=errors=0;world->EAUpdateFrame(&event);++scenarios;
        if(calls!=1 || reads || errors || memcmp(before,storage,sizeof(before))) return 1;
        calls=reads=errors=0;world->EAMoveHeroToRegion(&event);++scenarios;
        if(calls!=1 || reads || errors || memcmp(before,storage,sizeof(before))) return 2;
        for(unsigned length=1;length<=32;++length) {
            event.EndPos=length;memcpy(&eventBefore,&event,sizeof(event));expectedPause=pattern==1;
            calls=reads=errors=0;world->EASetPauseMode(&event);++scenarios;
#ifdef FABLE_WORLD_EVENT_REAL_PAUSE
            unsigned char pauseExpected[sizeof(storage)];memcpy(pauseExpected,before,sizeof(before));pauseExpected[8+0xa4]=expectedPause?1:0;
            if(calls || reads!=1 || errors || memcmp(pauseExpected,storage,sizeof(storage)) || memcmp(&event,&eventBefore,sizeof(event))) return 3;
#else
            if(calls!=1 || reads!=1 || errors || memcmp(before,storage,sizeof(before)) || memcmp(&event,&eventBefore,sizeof(event))) return 3;
#endif
            memcpy(storage,before,sizeof(storage));
            calls=reads=errors=0;world->EASetSlowMotion(&event);++scenarios;
            unsigned char expected[sizeof(storage)];memcpy(expected,before,sizeof(before));expected[8+0xa5]=expectedPause?1:0;
            if(calls || reads!=1 || errors || memcmp(expected,storage,sizeof(storage)) || memcmp(&event,&eventBefore,sizeof(event))) return 4;
            memcpy(storage,before,sizeof(storage));
        }
    }
    printf("WORLD_EVENT_CONTROL_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
