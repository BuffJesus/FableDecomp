#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
#include <stdio.h>
#include <string.h>
struct CPlayerLookupTestAccess{static void Setup(CPlayer& p,CPlayerManager& m,long number,long main){p.PlayerManager=&m;p.Number=number;m.MainPlayer=main;}};
typedef _STL::pair<ETCInterfaceType,CTCBase*> Entry;
struct CThingComponentTestAccess{static void Setup(CThing& t,Entry* e,unsigned n,bool has){t.UsingComponents.reset();t.UsingComponents.set(TCI_INVENTORY);t.UsingComponents.set(TCI_COMBAT,has);t.Components._M_start=e;t.Components._M_finish=e+n;t.Components._M_end_of_storage._M_data=e+n+1;}};
static CPlayerManager* expectedLookupManager;static CPlayer* expectedPlayer;static CThingPlayerCreature* creature;static CTCCombat* expectedCombat;static long expectedNumber;static unsigned calls,errors;
CPlayer& CPlayerManager::GetPlayer(long n){if(this!=expectedLookupManager || n!=expectedNumber || calls)++errors;++calls;return *expectedPlayer;}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{if(this!=(void*)((unsigned char*)expectedPlayer+0x2c) || calls!=1)++errors;++calls;return creature;}
void CTCCombat::UnsetBlocking(){if(this!=expectedCombat || calls!=2)++errors;++calls;}
static const long numbers[12]={0,1,-1,127,-128,(long)0x80000000UL,0x7fffffff,19,48,17,270,273};
static const long keys[5][7]={{0},{48},{(long)0x80000000UL,-1,17,48,48,49,0x7fffffff},{47,49},{0,17,47,54,273}};static const unsigned counts[5]={0,1,7,2,5};
int main(){unsigned scenarios=0;const unsigned char states[6]={0,0,1,255,2,254};
    for(unsigned pattern=0;pattern<4;++pattern){unsigned long ps[160],ms[32];memset(ps,pattern*71,sizeof(ps));memset(ms,pattern*71,sizeof(ms));CPlayer* p=reinterpret_cast<CPlayer*>(ps+2);CPlayerManager* m=reinterpret_cast<CPlayerManager*>(ms+2);
        for(unsigned n=0;n<12;++n)for(unsigned main=0;main<12;++main){CPlayerLookupTestAccess::Setup(*p,*m,numbers[n],numbers[main]);unsigned long pb[160],mb[32];memcpy(pb,ps,sizeof(ps));memcpy(mb,ms,sizeof(ms));if(p->IsMainPlayer()!=(numbers[n]==numbers[main]) || memcmp(pb,ps,sizeof(ps)) || memcmp(mb,ms,sizeof(ms)))return 1;++scenarios;}
        for(unsigned value=0;value<256;++value)for(unsigned state=0;state<6;++state)for(unsigned main=0;main<2;++main)for(unsigned has=0;has<2;++has)for(unsigned map=0;map<5;++map){unsigned long ws[104],cs[128];memset(ws,pattern*71,sizeof(ws));memset(cs,pattern*71,sizeof(cs));CWorld* world=reinterpret_cast<CWorld*>(ws+2);CThingPlayerCreature* backing=reinterpret_cast<CThingPlayerCreature*>(cs+2);creature=state==0?0:backing;reinterpret_cast<unsigned char*>(backing)[0x91]=states[state];expectedPlayer=p;expectedLookupManager=reinterpret_cast<CPlayerManager*>(0x14500000+pattern*0x1000);world->PlayerManager=expectedLookupManager;expectedNumber=(signed char)value;long ownNumber=numbers[value%12];long mainNumber=main?ownNumber:(long)((unsigned long)ownNumber^0x80000000UL);CPlayerLookupTestAccess::Setup(*p,*m,ownNumber,mainNumber);Entry entries[8],eb[8];memset(entries,pattern*71,sizeof(entries));unsigned found=counts[map];for(unsigned i=0;i<=counts[map];++i){entries[i].first=static_cast<ETCInterfaceType>(i<counts[map]?keys[map][i]:0x7fffffff);entries[i].second=reinterpret_cast<CTCBase*>(0x2500040+i*0x20);if(i<counts[map] && keys[map][i]==48 && found==counts[map])found=i;}expectedCombat=reinterpret_cast<CTCCombat*>(entries[found].second);CThingComponentTestAccess::Setup(*backing,entries,counts[map],has!=0);
            unsigned long wb[104],pb[160],mb[32],cb[128];memcpy(wb,ws,sizeof(ws));memcpy(pb,ps,sizeof(ps));memcpy(mb,ms,sizeof(ms));memcpy(cb,cs,sizeof(cs));memcpy(eb,entries,sizeof(entries));CGameEvent event(0x7fffffff);event.Player=(signed char)value;CGameEvent before;memcpy(&before,&event,sizeof(event));calls=errors=0;world->EAControlledCreatureBlockEnd(&event);bool unset=main && creature && !(states[state]&1) && has;if(errors || calls!=(unset?3:2) || memcmp(wb,ws,sizeof(ws)) || memcmp(pb,ps,sizeof(ps)) || memcmp(mb,ms,sizeof(ms)) || memcmp(cb,cs,sizeof(cs)) || memcmp(eb,entries,sizeof(entries)) || memcmp(&before,&event,sizeof(event)))return 2;++scenarios;
        }
    }
    printf("BLOCK_END_EVENT_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
