#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_gui.h"
#include <stdio.h>
#include <string.h>
template<typename T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
// Setup only: avoids allocating a counted ownership block around fixture storage.
template<typename T> void CCountedPointer<T>::Reset(T* data) {Data=data;Info=0;}
static CSubtitleRenderer* expected;
static unsigned calls,errors;
static bool result;
bool CSubtitleRenderer::ClickPast() {if(this!=expected) ++errors; ++calls; return result;}
int main() {
    unsigned scenarios=0;
    for(unsigned pattern=0;pattern<256;++pattern) {
        unsigned long guiStorage[104],worldStorage[104];memset(guiStorage,pattern,sizeof(guiStorage));memset(worldStorage,255-pattern,sizeof(worldStorage));
        CPlayerGui* gui=reinterpret_cast<CPlayerGui*>(&guiStorage[2]);CWorld* world=reinterpret_cast<CWorld*>(&worldStorage[2]);
        expected=reinterpret_cast<CSubtitleRenderer*>(reinterpret_cast<unsigned char*>(gui)+0x18);
        unsigned long guiBefore[104],worldBefore[104];memcpy(guiBefore,guiStorage,sizeof(guiStorage));memcpy(worldBefore,worldStorage,sizeof(worldStorage));
        CGameEvent event(0x7fffffff);event.Player=static_cast<signed char>(pattern);CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(gui);
        for(unsigned r=0;r<2;++r) {
            result=r!=0;calls=errors=0;world->EAClickPastText(r?&event:0);++scenarios;
            if(calls!=1 || errors || memcmp(guiBefore,guiStorage,sizeof(guiStorage)) || memcmp(worldBefore,worldStorage,sizeof(worldStorage)) || memcmp(&event,&eventBefore,sizeof(event))) return 1;
        }
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(0);
    }
    printf("CLICK_PAST_EVENT_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
