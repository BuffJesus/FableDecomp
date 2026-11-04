#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_gui.h"
#include "fable_player_manager.h"
#include <stdio.h>
#include <string.h>
template<typename T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template<typename T> void CCountedPointer<T>::Reset(T* data) {Data=data;Info=0;} // fixture setup only
struct CPlayerGuiHeroInfoTestAccess {static void Set(CPlayerGui& gui,NHeroInformationScreens::CManager* m) {memcpy(&gui.PHeroInfoScreensManager,&m,4);}};
namespace NHeroInformationScreens {struct HeroInformationManagerTestAccess {static void Set(CManager& m,unsigned char value) {memcpy(&m.OpenFlag,&value,1);}};}
static CWorld* expectedWorld;static CPlayer* expectedPlayer;static CPlayerManager* expectedManager;static long expectedNumber;static unsigned calls,errors;
CPlayer& CPlayerManager::GetPlayer(long number) {if(this!=expectedManager || number!=expectedNumber || calls || expectedWorld->IsPaused()) ++errors;++calls;return *expectedPlayer;}
void CPlayer::RemoveMode(EPlayerMode mode) {if(this!=expectedPlayer || mode!=PLAYER_MODE_NAVIGATE_IN_GAME_MENU || calls!=1 || expectedWorld->IsPaused()) ++errors;++calls;}
int main() {
    unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern) for(unsigned value=0;value<256;++value) for(unsigned openIndex=0;openIndex<4;++openIndex) for(unsigned pause=0;pause<2;++pause) {
        unsigned long worldStorage[104],guiStorage[224],managerStorage[32],playerStorage[144];memset(worldStorage,pattern*71,sizeof(worldStorage));memset(guiStorage,pattern*71,sizeof(guiStorage));memset(managerStorage,pattern*71,sizeof(managerStorage));memset(playerStorage,pattern*71,sizeof(playerStorage));
        CWorld* world=reinterpret_cast<CWorld*>(&worldStorage[2]);CPlayerGui* gui=reinterpret_cast<CPlayerGui*>(&guiStorage[2]);NHeroInformationScreens::CManager* manager=reinterpret_cast<NHeroInformationScreens::CManager*>(&managerStorage[2]);CPlayer* player=reinterpret_cast<CPlayer*>(&playerStorage[2]);
        expectedWorld=world;expectedPlayer=player;expectedManager=reinterpret_cast<CPlayerManager*>(0x14500000+pattern*0x1000);world->PlayerManager=expectedManager;world->Paused=pause!=0;expectedNumber=(signed char)value;
        CPlayerGuiHeroInfoTestAccess::Set(*gui,manager);static const unsigned char opens[4]={0,1,127,255};NHeroInformationScreens::HeroInformationManagerTestAccess::Set(*manager,opens[openIndex]);CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(gui);
        unsigned char worldExpected[sizeof(worldStorage)],guiBefore[sizeof(guiStorage)],managerExpected[sizeof(managerStorage)],playerBefore[sizeof(playerStorage)];memcpy(worldExpected,worldStorage,sizeof(worldStorage));worldExpected[8+0xa4]=0;memcpy(guiBefore,guiStorage,sizeof(guiStorage));memcpy(managerExpected,managerStorage,sizeof(managerStorage));managerExpected[8+0x34]=0;memcpy(playerBefore,playerStorage,sizeof(playerStorage));
        CGameEvent event(0x7fffffff);event.Player=(signed char)value;CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));calls=errors=0;world->EACloseInGameMenu(&event);++scenarios;
        if(calls!=2 || errors || memcmp(worldExpected,worldStorage,sizeof(worldStorage)) || memcmp(guiBefore,guiStorage,sizeof(guiStorage)) || memcmp(managerExpected,managerStorage,sizeof(managerStorage)) || memcmp(playerBefore,playerStorage,sizeof(playerStorage)) || memcmp(&eventBefore,&event,sizeof(event))) return 1;
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(0);
    }
    printf("CLOSE_IN_GAME_MENU_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
