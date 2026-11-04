#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
#include <stdio.h>
#include <string.h>
typedef _STL::pair<ETCInterfaceType,CTCBase*> Entry;
typedef CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Map;
struct CThingComponentTestAccess {static void Setup(CThing& thing,Entry* entries,unsigned count,bool has) {thing.UsingComponents.reset();thing.UsingComponents.set(TCI_INVENTORY,has);thing.Components._M_start=entries;thing.Components._M_finish=entries+count;thing.Components._M_end_of_storage._M_data=entries+count+1;}static Map& Get(CThing& thing) {return thing.Components;}};
static const long keys[5][7]={{0},{17},{(long)0x80000000UL,-1,0,17,17,18,0x7fffffff},{16,18},{0,1,16,48,54,273}};static const unsigned counts[5]={0,1,7,2,6};static const long queries[10]={(long)0x80000000UL,-1,0,1,16,17,18,273,274,0x7fffffff};
static CPlayerManager* expectedManager;static CPlayer* expectedPlayer;static CThingPlayerCreature* expectedCreature;static long expectedNumber;static unsigned expectedLeaf,calls,errors;static CTCInventory* expectedInventory;static CBaseObject* controlledResult;
CPlayer& CPlayerManager::GetPlayer(long number) {if(this!=expectedManager || number!=expectedNumber || calls) ++errors;++calls;return *expectedPlayer;}
CBaseObject* CBaseIntelligentPointer::GetPItem() const {if(this!=(void*)((unsigned char*)expectedPlayer+0x2c) || calls!=1) ++errors;++calls;return controlledResult;}
int main() {
    unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern) for(unsigned map=0;map<5;++map) {
        unsigned long worldStorage[104],playerStorage[144],creatureStorage[128];memset(worldStorage,pattern*71,sizeof(worldStorage));memset(playerStorage,pattern*71,sizeof(playerStorage));memset(creatureStorage,pattern*71,sizeof(creatureStorage));
        CWorld* world=reinterpret_cast<CWorld*>(&worldStorage[2]);expectedPlayer=reinterpret_cast<CPlayer*>(&playerStorage[2]);expectedCreature=reinterpret_cast<CThingPlayerCreature*>(&creatureStorage[2]);expectedManager=reinterpret_cast<CPlayerManager*>(0x14500000+pattern*0x1000);world->PlayerManager=expectedManager;
        Entry entries[8];for(unsigned i=0;i<counts[map]+1;++i) {entries[i].first=static_cast<ETCInterfaceType>(i<counts[map]?keys[map][i]:0x7fffffff);entries[i].second=reinterpret_cast<CTCBase*>(0x2500040+i*0x20);}
        for(unsigned has=0;has<2;++has) for(unsigned value=0;value<256;++value) for(unsigned leaf=0;leaf<4;++leaf) {
            CThingComponentTestAccess::Setup(*expectedCreature,entries,counts[map],has!=0);unsigned index=0;while(index<counts[map] && keys[map][index]!=17) ++index;expectedInventory=reinterpret_cast<CTCInventory*>(entries[index].second);expectedNumber=(signed char)value;expectedLeaf=leaf;
            unsigned long worldBefore[104],playerBefore[144],creatureBefore[128];Entry entriesBefore[8];memcpy(worldBefore,worldStorage,sizeof(worldStorage));memcpy(playerBefore,playerStorage,sizeof(playerStorage));memcpy(creatureBefore,creatureStorage,sizeof(creatureStorage));memcpy(entriesBefore,entries,sizeof(entries));
            controlledResult=reinterpret_cast<CBaseObject*>(leaf==0?0:leaf==1?(unsigned long)expectedCreature:leaf==2?0xfffffff0UL:0xdeadbeefUL);CGameEvent event(0x7fffffff);event.Player=(signed char)value;CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));calls=errors=0;
            world->EAOpenPCMSNChatMenu(&event);++scenarios;
            if(errors || calls!=2 || memcmp(worldBefore,worldStorage,sizeof(worldStorage)) || memcmp(playerBefore,playerStorage,sizeof(playerStorage)) || memcmp(creatureBefore,creatureStorage,sizeof(creatureStorage)) || memcmp(entriesBefore,entries,sizeof(entries)) || memcmp(&eventBefore,&event,sizeof(event))) return 2;
        }
    }
    printf("MSN_MENU_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}

