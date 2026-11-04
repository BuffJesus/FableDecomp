#include "fable_player_manager.h"
#include "fable_world.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>
struct CPlayerLookupTestAccess {
    static void Set(CPlayerManager* manager,CPlayer** entries) {
        manager->Players._M_start=entries;manager->Players._M_finish=entries+1;manager->Players._M_end_of_storage._M_data=entries+1;manager->PlayerNeutral=0;
    }
    static void Number(CPlayer* player,long number) {player->Number=number;}
};
struct CPlayerControlTestAccess {
    static void Set(CPlayer* player,CGamePlayerInterface* interface) {player->PlayerInterface=interface;}
    static unsigned char Suspended(CGamePlayerInterface* interface) {return *reinterpret_cast<unsigned char*>(&interface->Suspended);}
};
static CPlayer* expectedPlayer;
static CGamePlayerInterface* expectedInterface;
static unsigned char expectedSuspended;
static unsigned calls,errors;
void CPlayer::RemoveMode(EPlayerMode mode) {
    if(this!=expectedPlayer || mode!=PLAYER_MODE_FREEZE_CONTROLS || CPlayerControlTestAccess::Suspended(expectedInterface)!=expectedSuspended) ++errors;
    ++calls;
}
int main() {
    const unsigned values[8]={0,1,127,128,129,254,255,42};unsigned scenarios=0;
    for(unsigned suspended=0;suspended<256;++suspended)
    for(unsigned value=0;value<8;++value)
    for(unsigned match=0;match<2;++match)
    for(unsigned entry=0;entry<2;++entry) {
        unsigned long playerStorage[32],interfaceStorage[560],managerStorage[16],worldStorage[104];
        memset(playerStorage,suspended,sizeof(playerStorage));memset(interfaceStorage,255-suspended,sizeof(interfaceStorage));memset(managerStorage,suspended,sizeof(managerStorage));memset(worldStorage,255-suspended,sizeof(worldStorage));
        CPlayer* player=reinterpret_cast<CPlayer*>(&playerStorage[2]);CGamePlayerInterface* interface=reinterpret_cast<CGamePlayerInterface*>(&interfaceStorage[2]);
        CPlayerManager* manager=reinterpret_cast<CPlayerManager*>(&managerStorage[2]);CWorld* world=reinterpret_cast<CWorld*>(&worldStorage[2]);world->PlayerManager=manager;
        CGameEvent event(0x7fffffff);event.Player=static_cast<signed char>(values[value]);CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));
        CPlayer* entries[1]={player};CPlayerLookupTestAccess::Set(manager,entries);CPlayerLookupTestAccess::Number(player,match?event.Player:256+values[value]);
        CPlayerControlTestAccess::Set(player,interface);reinterpret_cast<unsigned char*>(interface)[0x79c]=suspended;
        expectedPlayer=player;expectedInterface=interface;expectedSuspended=suspended;calls=errors=0;
        unsigned long playerBefore[32],managerBefore[16],worldBefore[104];unsigned char interfaceExpected[sizeof(interfaceStorage)];
        memcpy(playerBefore,playerStorage,sizeof(playerStorage));memcpy(managerBefore,managerStorage,sizeof(managerStorage));memcpy(worldBefore,worldStorage,sizeof(worldStorage));memcpy(interfaceExpected,interfaceStorage,sizeof(interfaceStorage));interfaceExpected[8+0x79c]=0;
        if(entry) world->EAControlledCreatureUnfreezeControls(&event);else player->UnfreezeControls();++scenarios;
        if(calls!=1 || errors || memcmp(playerBefore,playerStorage,sizeof(playerStorage)) || memcmp(managerBefore,managerStorage,sizeof(managerStorage)) || memcmp(worldBefore,worldStorage,sizeof(worldStorage)) || memcmp(interfaceExpected,interfaceStorage,sizeof(interfaceStorage)) || memcmp(&event,&eventBefore,sizeof(event))) return 1;
    }
    printf("PLAYER_UNFREEZE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
