#include "fable_player_manager.h"
#include "fable_world.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>
struct CPlayerLookupTestAccess {
    static void Set(CPlayerManager* manager,CPlayer** entries) {manager->Players._M_start=entries;manager->Players._M_finish=entries+1;manager->Players._M_end_of_storage._M_data=entries+1;manager->PlayerNeutral=0;}
    static void Number(CPlayer* p,long n) {p->Number=n;}
};
struct CPlayerControlTestAccess {
    static void Mode(CPlayer* p,unsigned char value) {*reinterpret_cast<unsigned char*>(&p->AggressiveMode)=value;}
    static unsigned char Mode(CPlayer* p) {return *reinterpret_cast<unsigned char*>(&p->AggressiveMode);}
};
static CPlayer* expectedPlayer;
static unsigned expectedMode,calls,errors;
void CPlayer::SetUpControlsBasedOnAggressiveMode() {
    if(this!=expectedPlayer || CPlayerControlTestAccess::Mode(this)!=expectedMode) ++errors; ++calls;
}
static bool Run(unsigned enable,unsigned oldMode,bool composed,unsigned playerValue) {
    unsigned long playerStorage[160],managerStorage[16],worldStorage[104];memset(playerStorage,oldMode,sizeof(playerStorage));memset(managerStorage,255-oldMode,sizeof(managerStorage));memset(worldStorage,oldMode,sizeof(worldStorage));
    CPlayer* player=reinterpret_cast<CPlayer*>(&playerStorage[2]);CPlayerManager* manager=reinterpret_cast<CPlayerManager*>(&managerStorage[2]);CWorld* world=reinterpret_cast<CWorld*>(&worldStorage[2]);world->PlayerManager=manager;
    CGameEvent event(0x7fffffff);event.Player=static_cast<signed char>(playerValue);CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));
    CPlayer* entries[1]={player};CPlayerLookupTestAccess::Set(manager,entries);CPlayerLookupTestAccess::Number(player,playerValue&1?event.Player:256+playerValue);CPlayerControlTestAccess::Mode(player,oldMode);
    *reinterpret_cast<unsigned char*>(&GUsePassiveAggressiveMode)=enable;
    expectedPlayer=player;expectedMode=enable?(oldMode==0?1:0):oldMode;calls=errors=0;
    unsigned char playerExpected[sizeof(playerStorage)];unsigned long managerBefore[16],worldBefore[104];memcpy(playerExpected,playerStorage,sizeof(playerStorage));playerExpected[8+0x217]=expectedMode;memcpy(managerBefore,managerStorage,sizeof(managerStorage));memcpy(worldBefore,worldStorage,sizeof(worldStorage));
    if(composed) world->EATogglePassiveAggressiveMode(&event);else player->ToggleAggressiveMode();
    return calls==unsigned(enable!=0) && !errors && *reinterpret_cast<unsigned char*>(&GUsePassiveAggressiveMode)==enable && !memcmp(playerExpected,playerStorage,sizeof(playerStorage)) && !memcmp(managerBefore,managerStorage,sizeof(managerStorage)) && !memcmp(worldBefore,worldStorage,sizeof(worldStorage)) && !memcmp(&event,&eventBefore,sizeof(event));
}
int main() {
    if(GUsePassiveAggressiveMode) return 1;
    const unsigned enabled[6]={0,1,2,127,128,255},values[8]={0,1,127,128,129,254,255,42};unsigned direct=0,composed=0;
    for(unsigned old=0;old<256;++old) {
        for(unsigned i=0;i<6;++i) {if(!Run(enabled[i],old,false,0)) return 2;++direct;}
        for(unsigned e=0;e<2;++e) for(i=0;i<8;++i) {if(!Run(e,old,true,values[i])) return 3;++composed;}
    }
    GUsePassiveAggressiveMode=false;
    printf("PLAYER_AGGRESSIVE_BEHAVIOR_PASS direct=%u composed=%u\n",direct,composed);return 0;
}
