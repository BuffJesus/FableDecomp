#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
#include <stdio.h>
#include <string.h>

class WorldAccess : public CWorld { public: using CWorld::GetPlayer; };
// Recording boundaries use storage; no native CPlayer constructor is invoked.
static unsigned long playerStorage[4][16];
static CPlayer* PlayerAt(unsigned long number) {
    return reinterpret_cast<CPlayer*>(playerStorage[number&3]);
}
static CPlayerManager* expectedManager;
static long expectedNumber;
static CPlayer* expectedPlayer;
static unsigned lookups, actions, errors;
static int expectedKind, expectedMode;
CPlayer& CPlayerManager::GetPlayer(long number) {
    if(this!=expectedManager || number!=expectedNumber) ++errors;
    ++lookups;return *PlayerAt(static_cast<unsigned long>(number));
}
void CPlayer::AddMode(EPlayerMode mode,bool force) {
    if(this!=expectedPlayer || lookups!=1 || expectedKind!=1 || mode!=expectedMode || force) ++errors; ++actions;
}
void CPlayer::RemoveMode(EPlayerMode mode) {
    if(this!=expectedPlayer || lookups!=1 || expectedKind!=2 || mode!=expectedMode) ++errors; ++actions;
}
void CPlayer::UnfreezeControls() {
    if(this!=expectedPlayer || lookups!=1 || expectedKind!=3) ++errors; ++actions;
}
void CPlayer::ToggleAggressiveMode() {
    if(this!=expectedPlayer || lookups!=1 || expectedKind!=4) ++errors; ++actions;
}
CThingPlayerCreature* CPlayer::GetPControlledCreature() const {
    if(this!=expectedPlayer || lookups!=1 || expectedKind!=5) ++errors; ++actions;return 0;
}
typedef void (CWorld::*EventMethod)(const CGameEvent*);
int main() {
    EventMethod methods[8]={&CWorld::EAControlledCreatureUnfreezeControls,&CWorld::EAControlledCreatureStartThrowObject,&CWorld::EAFirstPersonViewStart,&CWorld::EAFirstPersonTargeting,&CWorld::EAControlledCreatureDeactivateZTarget,&CWorld::EAControlledCreatureDeactivateLightning,&CWorld::EATogglePassiveAggressiveMode,&CWorld::EAOpenPCMSNChatMenu};
    const int kinds[8]={3,1,1,1,2,2,4,5},modes[8]={0,16,16,22,2,40,0,0};
    unsigned long numbers[8]={0,1,127,128,255,0xffffffff,0x80000000,0x7fffffff};
    unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern) {
        unsigned long storage[104];memset(storage,pattern*71,sizeof(storage));
        WorldAccess* world=reinterpret_cast<WorldAccess*>(&storage[2]);
        expectedManager=reinterpret_cast<CPlayerManager*>(0x12345000+pattern*0x1000);world->PlayerManager=expectedManager;
        unsigned char before[sizeof(storage)];memcpy(before,storage,sizeof(storage));
        for(unsigned i=0;i<8;++i) {
            expectedNumber=static_cast<long>(numbers[i]);lookups=actions=errors=0;
            CPlayer* player=world->GetPlayer(numbers[i]);++scenarios;
            if(player!=PlayerAt(numbers[i]) || lookups!=1 || actions || errors || memcmp(before,storage,sizeof(storage))) return 1;
        }
        for(unsigned value=0;value<256;++value) {
            CGameEvent event(0x7fffffff);event.Player=static_cast<signed char>(value);memset(event.Data,value,sizeof(event.Data));
            CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));
            expectedNumber=event.Player;expectedPlayer=PlayerAt(static_cast<unsigned long>(expectedNumber));
            for(i=0;i<8;++i) {
                expectedKind=kinds[i];expectedMode=modes[i];lookups=actions=errors=0;
                (world->*methods[i])(&event);++scenarios;
                if(lookups!=1 || actions!=1 || errors || memcmp(before,storage,sizeof(storage)) || memcmp(&event,&eventBefore,sizeof(event))) return 2;
            }
        }
    }
    printf("PLAYER_EVENTS_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
