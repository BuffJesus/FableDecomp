#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
#include <stdio.h>
#include <string.h>
struct CPlayerLookupTestAccess{static void Setup(CPlayer& p,_STL::_List_node<EPlayerMode>* sentinel){p.PlayerModes._M_node=sentinel;}};
static CPlayerManager* expectedManager;static CPlayer* expectedPlayer;static CThingPlayerCreature* creature;static long number;static unsigned calls,errors;static bool shouldAdd;
CPlayer& CPlayerManager::GetPlayer(long n){if(this!=expectedManager || n!=number || calls)++errors;++calls;return *expectedPlayer;}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{if(this!=(void*)((unsigned char*)expectedPlayer+0x2c) || calls!=1)++errors;++calls;return creature;}
void CPlayer::AddMode(EPlayerMode m,bool force){if(this!=expectedPlayer || !shouldAdd || m!=PLAYER_MODE_CONTROL_CREATURE || !force || calls!=2)++errors;++calls;}
static const long modes[12]={1,0,2,19,-1,(long)0x80000000UL,0x7fffffff,16,17,40,43,273};
int main(){unsigned scenarios=0;const unsigned counts[3]={0,1,3};const unsigned char states[6]={0,0,1,255,2,254};
    for(unsigned pattern=0;pattern<4;++pattern)for(unsigned m=0;m<12;++m)for(unsigned count=0;count<3;++count){unsigned long ws[104],ps[160],cs[128];memset(ws,pattern*71,sizeof(ws));memset(ps,pattern*71,sizeof(ps));memset(cs,pattern*71,sizeof(cs));CWorld* world=reinterpret_cast<CWorld*>(ws+2);expectedPlayer=reinterpret_cast<CPlayer*>(ps+2);expectedManager=reinterpret_cast<CPlayerManager*>(0x14500000+pattern*0x1000);world->PlayerManager=expectedManager;
        _STL::_List_node<EPlayerMode> nodes[4],nb[4];memset(nodes,pattern*71,sizeof(nodes));unsigned n=counts[count];nodes[0]._M_next=n?nodes+1:nodes;nodes[0]._M_prev=nodes+n;nodes[0]._M_data=static_cast<EPlayerMode>(modes[m]);for(unsigned i=1;i<=n;++i){nodes[i]._M_prev=nodes+i-1;nodes[i]._M_next=i<n?nodes+i+1:nodes;nodes[i]._M_data=static_cast<EPlayerMode>(i==n?modes[m]:40+i);}CPlayerLookupTestAccess::Setup(*expectedPlayer,nodes);memcpy(nb,nodes,sizeof(nodes));unsigned long pb[160];memcpy(pb,ps,sizeof(ps));
        if(expectedPlayer->GetCurrentMode()!=static_cast<EPlayerMode>(modes[m]) || memcmp(pb,ps,sizeof(ps)) || memcmp(nb,nodes,sizeof(nodes)))return 1;++scenarios;
        for(unsigned state=0;state<6;++state)for(unsigned value=0;value<256;++value){creature=state==0?0:reinterpret_cast<CThingPlayerCreature*>(cs+2);reinterpret_cast<unsigned char*>(cs+2)[0x91]=states[state];number=(signed char)value;shouldAdd=creature && !(states[state]&1) && modes[m]!=1;unsigned long wb[104],cb[128];memcpy(wb,ws,sizeof(ws));memcpy(cb,cs,sizeof(cs));CGameEvent event(0x7fffffff);event.Player=(signed char)value;CGameEvent eb;memcpy(&eb,&event,sizeof(event));calls=errors=0;world->EAControlledCreatureDefault(&event);if(errors || calls!=(shouldAdd?3:2) || memcmp(wb,ws,sizeof(ws)) || memcmp(pb,ps,sizeof(ps)) || memcmp(cb,cs,sizeof(cs)) || memcmp(nb,nodes,sizeof(nodes)) || memcmp(&eb,&event,sizeof(event)))return 2;++scenarios;}
    }
    printf("PLAYER_DEFAULT_MODE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
