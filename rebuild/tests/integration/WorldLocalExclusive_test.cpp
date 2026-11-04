#include "fable_world.h"
#include "fable_game.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>
static CMainGameComponent* expectedComponent;
static unsigned calls,errors;
static bool expectedExclusive;
void CMainGameComponent::SetExclusiveMode(bool value) {if(this!=expectedComponent || value!=expectedExclusive) ++errors;++calls;}
int main() {
    unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern) {
        unsigned long storage[104];memset(storage,pattern*71,sizeof(storage));CWorld* world=reinterpret_cast<CWorld*>(&storage[2]);expectedComponent=reinterpret_cast<CMainGameComponent*>(0x12345678+pattern*0x1000);memcpy((unsigned char*)world+8,&expectedComponent,4);
        unsigned long before[104];memcpy(before,storage,sizeof(before));
        for(unsigned value=0;value<256;++value) {
            CGameEvent event(0x7fffffff);memset(event.Data,value,sizeof(event.Data));event.Player=(signed char)(255-value);expectedExclusive=value!=0;
            for(unsigned length=1;length<=32;++length) {
                event.EndPos=length;CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));calls=errors=0;world->LEASetExclusiveMode(&event);++scenarios;
                if(calls!=1 || errors || memcmp(before,storage,sizeof(before)) || memcmp(&event,&eventBefore,sizeof(event))) return 1;
            }
        }
    }
    printf("WORLD_LOCAL_EXCLUSIVE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
