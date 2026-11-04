#include "fable_player_manager.h"
#include "fable_world.h"
#include <stdio.h>
#include <string.h>
struct WorldLookupAccess : public CWorld { public: using CWorld::GetPlayer; };
struct CPlayerLookupTestAccess {
    static void Number(CPlayer* p,long n) {p->Number=n;}
    static void Set(CPlayerManager* manager,CPlayer** entries,unsigned count,long neutral,long main) {
        manager->Players._M_start=entries;manager->Players._M_finish=entries+count;manager->Players._M_end_of_storage._M_data=entries+8;
        manager->PlayerNeutral=neutral;manager->MainPlayer=main;
    }
};
int main() {
    const long numbers[4][8]={{0,1,2,3,4,5,6,7},{0,-1,static_cast<long>(0x80000000),0x7fffffff,7,7,127,128},{7,7,7,7,7,7,7,7},{255,-128,127,-1,0,1,42,-42}};
    const long queries[12]={0,1,7,42,-1,127,-128,255,128,static_cast<long>(0x80000000),0x7fffffff,-42};
    unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern)
    for(unsigned count=0;count<=8;++count)
    for(unsigned neutral=0;neutral<(count?count:1);++neutral)
    for(unsigned query=0;query<12;++query) {
        unsigned long managerStorage[16],worldStorage[104],playerStorage[8][32];
        memset(managerStorage,pattern*71,sizeof(managerStorage));memset(worldStorage,255-pattern*71,sizeof(worldStorage));memset(playerStorage,pattern*71,sizeof(playerStorage));
        CPlayerManager* manager=reinterpret_cast<CPlayerManager*>(&managerStorage[2]);WorldLookupAccess* world=reinterpret_cast<WorldLookupAccess*>(&worldStorage[2]);world->PlayerManager=manager;
        CPlayer* entries[8];
        for(unsigned i=0;i<8;++i) {entries[i]=reinterpret_cast<CPlayer*>(&playerStorage[i][2]);CPlayerLookupTestAccess::Number(entries[i],numbers[pattern][i]);}
        CPlayerLookupTestAccess::Set(manager,entries,count,neutral,queries[query]);
        CPlayer* expected=entries[neutral];for(i=0;i<count;++i) if(numbers[pattern][i]==queries[query]) {expected=entries[i];break;}
        unsigned long managerBefore[16],worldBefore[104],playersBefore[8][32];memcpy(managerBefore,managerStorage,sizeof(managerStorage));memcpy(worldBefore,worldStorage,sizeof(worldStorage));memcpy(playersBefore,playerStorage,sizeof(playerStorage));
        if(&manager->GetPlayer(queries[query])!=expected) return 1;++scenarios;
        const CPlayerManager* view=manager;
        if(&view->GetPlayer(queries[query])!=expected) return 2;++scenarios;
        if(&manager->GetMainPlayer()!=expected) return 3;++scenarios;
        if(&view->GetMainPlayer()!=expected) return 4;++scenarios;
        if(view->GetMainPlayerNumber()!=queries[query]) return 5;++scenarios;
        if(world->GetPlayer(static_cast<unsigned long>(queries[query]))!=expected) return 6;++scenarios;
        if(memcmp(managerBefore,managerStorage,sizeof(managerStorage)) || memcmp(worldBefore,worldStorage,sizeof(worldStorage)) || memcmp(playersBefore,playerStorage,sizeof(playerStorage))) return 7;
    }
    printf("PLAYER_LOOKUP_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
