#include "fable_world.h"
#include "fable_player_manager.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>
struct CPlayerLookupTestAccess {
    static void Set(CPlayerManager* m,CPlayer** entries) {m->Players._M_start=entries;m->Players._M_finish=entries+1;m->Players._M_end_of_storage._M_data=entries+1;m->PlayerNeutral=0;}
    static void Number(CPlayer* p,long n) {p->Number=n;}
};
struct CPlayerControlTestAccess {
    static void Mode(CPlayer* p,unsigned char n) {*reinterpret_cast<unsigned char*>(&p->KillEverythingMode)=n;}
};
int main() {
    const unsigned values[]={0,1,2,15,16,40,42,63,64,126,127,128,129,253,254,255};unsigned scenarios=0;
    for(unsigned old=0;old<256;++old)
    for(unsigned value=0;value<16;++value)
    for(unsigned match=0;match<2;++match) {
        unsigned long player[160],manager[16],world[104];memset(player,old,sizeof(player));memset(manager,255-old,sizeof(manager));memset(world,old,sizeof(world));
        CPlayer* p=reinterpret_cast<CPlayer*>(&player[2]);CPlayerManager* m=reinterpret_cast<CPlayerManager*>(&manager[2]);CWorld* w=reinterpret_cast<CWorld*>(&world[2]);w->PlayerManager=m;
        CGameEvent event(0x7fffffff),eventBefore;event.Player=static_cast<signed char>(values[value]);memcpy(&eventBefore,&event,sizeof(event));
        CPlayer* entries[]={p};CPlayerLookupTestAccess::Set(m,entries);CPlayerLookupTestAccess::Number(p,match?event.Player:256+values[value]);CPlayerControlTestAccess::Mode(p,old);
        unsigned char expected[sizeof(player)];unsigned long mBefore[16],wBefore[104];memcpy(expected,player,sizeof(player));expected[8+0x21b]=old==0;memcpy(mBefore,manager,sizeof(manager));memcpy(wBefore,world,sizeof(world));
        w->EAToggleKillEverythingMode(&event);++scenarios;
        if(memcmp(expected,player,sizeof(player)) || memcmp(mBefore,manager,sizeof(manager)) || memcmp(wBefore,world,sizeof(world)) || memcmp(&event,&eventBefore,sizeof(event))) return 1;
    }
    printf("WORLD_KILL_MODE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
