#include "fable_world.h"
#include "fable_player_manager.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
static void* expectedDelete;
static CThing* expectedThing;
static unsigned deletes,kills,errors;
void* operator new(size_t n) {return malloc(n);}
void operator delete(void* p) {if(p!=expectedDelete) ++errors;++deletes;free(p);}
void CThing::Kill(bool force) {if(this!=expectedThing || !force) ++errors;++kills;}
struct CBaseIntelligentPointerTestAccess {static void Set(CBaseIntelligentPointer* p,CBaseObjectPointer* d) {p->PData=d;}};
struct CPlayerControlTestAccess {static CBaseIntelligentPointer* Pointer(CPlayer* p) {return &p->ControlledCreature;}};
struct CPlayerLookupTestAccess {
    static void Set(CPlayerManager* m,CPlayer** entries,long number) {m->Players._M_start=entries;m->Players._M_finish=entries+1;m->Players._M_end_of_storage._M_data=entries+1;m->PlayerNeutral=0;m->MainPlayer=number;}
    static void Number(CPlayer* p,long n) {p->Number=n;}
};
int main() {
    const unsigned long counts[]={0,1,2,0xffffffff};unsigned scenarios=0;
    for(unsigned state=0;state<3;++state)
    for(unsigned count=0;count<4;++count)
    for(unsigned value=0;value<256;++value)
    for(unsigned match=0;match<2;++match) {
        unsigned long player[144],creature[48],manager[16],world[104];memset(player,0xa9,sizeof(player));memset(creature,0xb7,sizeof(creature));memset(manager,0xc5,sizeof(manager));memset(world,0xd3,sizeof(world));
        CPlayer* p=reinterpret_cast<CPlayer*>(&player[2]);CThingPlayerCreature* target=reinterpret_cast<CThingPlayerCreature*>(&creature[2]);CPlayerManager* m=reinterpret_cast<CPlayerManager*>(&manager[2]);CWorld* w=reinterpret_cast<CWorld*>(&world[2]);w->PlayerManager=m;
        CGameEvent event(0),eventBefore;event.Player=static_cast<signed char>(value);memcpy(&eventBefore,&event,sizeof(event));CPlayer* entries[]={p};long mainNumber=static_cast<long>(value)*1234567L;CPlayerLookupTestAccess::Set(m,entries,mainNumber);CPlayerLookupTestAccess::Number(p,match?mainNumber:mainNumber+1);
        CBaseObjectPointer* data=static_cast<CBaseObjectPointer*>(malloc(8));data->object=state==2?target:0;data->refcount=counts[count];CBaseIntelligentPointerTestAccess::Set(CPlayerControlTestAccess::Pointer(p),state?data:0);
        unsigned long expected[144],cBefore[48],mBefore[16],wBefore[104];memcpy(expected,player,sizeof(player));if(state==1) expected[2+0x30/4]=0;memcpy(cBefore,creature,sizeof(creature));memcpy(mBefore,manager,sizeof(manager));memcpy(wBefore,world,sizeof(world));
        expectedDelete=state==1&&counts[count]==1?data:0;expectedThing=target;deletes=kills=errors=0;
        w->EACreatureCheat(&event);++scenarios;
        if(deletes!=unsigned(expectedDelete!=0) || kills!=unsigned(state==2) || errors || memcmp(expected,player,sizeof(player)) || memcmp(cBefore,creature,sizeof(creature)) || memcmp(mBefore,manager,sizeof(manager)) || memcmp(wBefore,world,sizeof(world)) || memcmp(&event,&eventBefore,sizeof(event))) return 1;
        if(!expectedDelete) {if(data->refcount!=counts[count]-unsigned(state==1)) return 2;free(data);}
    }
    printf("WORLD_CREATURE_CHEAT_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
