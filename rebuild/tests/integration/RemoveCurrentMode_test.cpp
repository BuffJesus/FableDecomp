#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
#include <stdio.h>
#include <string.h>
typedef _STL::pair<ETCInterfaceType,CTCBase*> Entry;
struct CThingComponentTestAccess{static void Setup(CThing& t,Entry* e,unsigned n,bool has){t.UsingComponents.reset();t.UsingComponents.set(TCI_INVENTORY);t.UsingComponents.set(TCI_HERO_RANGED_ATTACKS,has);t.Components._M_start=e;t.Components._M_finish=e+n;t.Components._M_end_of_storage._M_data=e+n+1;}};
static CPlayerManager* expectedManager;static CPlayer* expectedPlayer;static CThingPlayerCreature* creature;static CTCHeroRangedAttacks* expectedRanged;static long expectedNumber,expectedMode;static unsigned calls,errors,mutation;static unsigned long* expectedPlayerBytes;static unsigned long* expectedCreatureBytes;
CPlayer& CPlayerManager::GetPlayer(long n){if(this!=expectedManager || n!=expectedNumber || calls)++errors;++calls;return *expectedPlayer;}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{if(this!=(void*)((unsigned char*)expectedPlayer+0x2c) || calls!=1)++errors;++calls;return creature;}
void CPlayer::RemoveMode(EPlayerMode mode){if(this!=expectedPlayer || mode!=expectedMode || calls!=2)++errors;++calls;if(mutation==1){unsigned long marker=0xdeadbeef;memcpy((unsigned char*)this+0x2c,&marker,4);memcpy((unsigned char*)expectedPlayerBytes+8+0x2c,&marker,4);}if(mutation==2 && creature){((unsigned char*)creature)[0x91]^=1;((unsigned char*)expectedCreatureBytes)[8+0x91]^=1;}}
void CTCHeroRangedAttacks::ResetProjectileWeaponMode(){if(this!=expectedRanged || calls!=3)++errors;++calls;}
static const long modes[6]={0,1,-1,(long)0x80000000UL,0x7fffffff,129};
static const long keys[5][7]={{0},{129},{(long)0x80000000UL,-1,17,129,129,130,0x7fffffff},{128,130},{0,17,48,54,273}};static const unsigned counts[5]={0,1,7,2,5};
int main(){unsigned scenarios=0;const unsigned char states[6]={0,0,1,255,2,254};const unsigned lengths[3]={4,8,32};
    for(unsigned pattern=0;pattern<2;++pattern)for(unsigned value=0;value<256;++value)for(unsigned mode=0;mode<6;++mode)for(unsigned state=0;state<6;++state)for(unsigned has=0;has<2;++has)for(unsigned map=0;map<5;++map)for(unsigned length=0;length<3;++length)for(mutation=0;mutation<3;++mutation){
        unsigned long ws[104],ps[160],cs[128];memset(ws,pattern*71,sizeof(ws));memset(ps,pattern*71,sizeof(ps));memset(cs,pattern*71,sizeof(cs));CWorld* world=reinterpret_cast<CWorld*>(ws+2);expectedPlayer=reinterpret_cast<CPlayer*>(ps+2);CThingPlayerCreature* backing=reinterpret_cast<CThingPlayerCreature*>(cs+2);creature=state==0?0:backing;reinterpret_cast<unsigned char*>(backing)[0x91]=states[state];expectedManager=reinterpret_cast<CPlayerManager*>(0x14500000+pattern*0x1000);world->PlayerManager=expectedManager;expectedNumber=(signed char)value;expectedMode=modes[mode];
        Entry entries[8],eb[8];memset(entries,pattern*71,sizeof(entries));unsigned found=counts[map];for(unsigned i=0;i<=counts[map];++i){entries[i].first=static_cast<ETCInterfaceType>(i<counts[map]?keys[map][i]:0x7fffffff);entries[i].second=reinterpret_cast<CTCBase*>(0x2500040+i*0x20);if(i<counts[map] && keys[map][i]==129 && found==counts[map])found=i;}expectedRanged=reinterpret_cast<CTCHeroRangedAttacks*>(entries[found].second);CThingComponentTestAccess::Setup(*backing,entries,counts[map],has!=0);
        unsigned long wb[104],pb[160],cb[128];memcpy(wb,ws,sizeof(ws));memcpy(pb,ps,sizeof(ps));memcpy(cb,cs,sizeof(cs));memcpy(eb,entries,sizeof(entries));expectedPlayerBytes=pb;expectedCreatureBytes=cb;
        unsigned long es[16],before[16];memset(es,pattern*71,sizeof(es));CGameEvent* event=reinterpret_cast<CGameEvent*>(es+2);event->Player=(signed char)value;memcpy(event->Data,&expectedMode,4);event->EndPos=(unsigned char)lengths[length];memcpy(before,es,sizeof(es));calls=errors=0;world->EARemoveCurrentMode(event);bool reset=creature && !(((unsigned char*)backing)[0x91]&1) && has;
        if(errors || calls!=(reset?4:3) || memcmp(wb,ws,sizeof(ws)) || memcmp(pb,ps,sizeof(ps)) || memcmp(cb,cs,sizeof(cs)) || memcmp(eb,entries,sizeof(entries)) || memcmp(before,es,sizeof(es)))return 1;++scenarios;
    }
    printf("REMOVE_CURRENT_MODE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
