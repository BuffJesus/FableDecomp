#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_gui.h"
#include "fable_player_manager.h"
#include <stdio.h>
#include <string.h>
template<typename T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template<typename T> void CCountedPointer<T>::Reset(T* data){Data=data;Info=0;}
struct CPlayerGuiHeroInfoTestAccess{static void Set(CPlayerGui& g,NHeroInformationScreens::CManager* m){memcpy(&g.PHeroInfoScreensManager,&m,4);}};
namespace NHeroInformationScreens{struct HeroInformationManagerTestAccess{static void Set(CManager& m,unsigned char v){memcpy(&m.OpenFlag,&v,1);}static bool Closed(const CManager& m){return !m.OpenFlag;}};}
typedef _STL::pair<ETCInterfaceType,CTCBase*> Entry;
struct CThingComponentTestAccess{static void Set(CThing& t,Entry* e,unsigned n,bool has){t.UsingComponents.reset();t.UsingComponents.set(TCI_INVENTORY);t.UsingComponents.set(TCI_PHOTOJOURNAL_CAPTURE,has);t.Components._M_start=e;t.Components._M_finish=e+n;t.Components._M_end_of_storage._M_data=e+n+1;}};
static CWorld* expectedWorld;static CPlayer* expectedPlayer;static CPlayerManager* expectedManager;static CThingPlayerCreature* expectedCreature;static NHeroInformationScreens::CManager* heroManager;static CTCPhotojournalCapture* capture;static long number;static unsigned calls,errors;
CPlayer& CPlayerManager::GetPlayer(long n){if(this!=expectedManager || n!=number || calls || expectedWorld->IsPaused())++errors;++calls;return *expectedPlayer;}
void CPlayer::RemoveMode(EPlayerMode m){if(this!=expectedPlayer || m!=PLAYER_MODE_TAKE_SCREENSHOT_FOR_PHOTOJOURNAL || calls!=1 || expectedWorld->IsPaused())++errors;++calls;}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{if(this!=(void*)((unsigned char*)expectedPlayer+0x2c) || calls!=2 || !NHeroInformationScreens::HeroInformationManagerTestAccess::Closed(*heroManager))++errors;++calls;return expectedCreature;}
void CTCPhotojournalCapture::DestroyScreen(){if(this!=capture || calls!=3)++errors;++calls;}
static const long keys[5][7]={{0},{270},{(long)0x80000000UL,-1,17,270,270,271,0x7fffffff},{269,271},{0,17,48,54,269,273}};static const unsigned counts[5]={0,1,7,2,6};
int main(){unsigned scenarios=0;const unsigned char opens[4]={0,1,127,255};
    for(unsigned pattern=0;pattern<4;++pattern)for(unsigned map=0;map<5;++map)for(unsigned present=0;present<2;++present)for(unsigned pause=0;pause<2;++pause)for(unsigned opened=0;opened<4;++opened)for(unsigned value=0;value<256;++value){unsigned long ws[104],gs[224],ms[32],ps[160],cs[128];memset(ws,pattern*71,sizeof(ws));memset(gs,pattern*71,sizeof(gs));memset(ms,pattern*71,sizeof(ms));memset(ps,pattern*71,sizeof(ps));memset(cs,pattern*71,sizeof(cs));expectedWorld=reinterpret_cast<CWorld*>(ws+2);CPlayerGui* gui=reinterpret_cast<CPlayerGui*>(gs+2);heroManager=reinterpret_cast<NHeroInformationScreens::CManager*>(ms+2);expectedPlayer=reinterpret_cast<CPlayer*>(ps+2);expectedCreature=reinterpret_cast<CThingPlayerCreature*>(cs+2);expectedManager=reinterpret_cast<CPlayerManager*>(0x14500000+pattern*0x1000);expectedWorld->PlayerManager=expectedManager;expectedWorld->Paused=pause!=0;number=(signed char)value;CPlayerGuiHeroInfoTestAccess::Set(*gui,heroManager);NHeroInformationScreens::HeroInformationManagerTestAccess::Set(*heroManager,opens[opened]);CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(gui);
        Entry entries[8],eb[8];memset(entries,pattern*71,sizeof(entries));unsigned found=counts[map];for(unsigned i=0;i<=counts[map];++i){entries[i].first=static_cast<ETCInterfaceType>(i<counts[map]?keys[map][i]:0x7fffffff);entries[i].second=reinterpret_cast<CTCBase*>(0x2500040+i*0x20);if(i<counts[map] && keys[map][i]==270 && found==counts[map])found=i;}capture=reinterpret_cast<CTCPhotojournalCapture*>(entries[found].second);CThingComponentTestAccess::Set(*expectedCreature,entries,counts[map],present!=0);
        unsigned long wb[104],gb[224],mb[32],pb[160],cb[128];memcpy(wb,ws,sizeof(ws));reinterpret_cast<unsigned char*>(wb+2)[0xa4]=0;memcpy(gb,gs,sizeof(gs));memcpy(mb,ms,sizeof(ms));reinterpret_cast<unsigned char*>(mb+2)[0x34]=0;memcpy(pb,ps,sizeof(ps));memcpy(cb,cs,sizeof(cs));memcpy(eb,entries,sizeof(entries));CGameEvent event(0x7fffffff);event.Player=(signed char)value;CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));calls=errors=0;expectedWorld->EAClosePhotoCapture(&event);
        if(errors || calls!=(present?4:3) || memcmp(wb,ws,sizeof(ws)) || memcmp(gb,gs,sizeof(gs)) || memcmp(mb,ms,sizeof(ms)) || memcmp(pb,ps,sizeof(ps)) || memcmp(cb,cs,sizeof(cs)) || memcmp(eb,entries,sizeof(entries)) || memcmp(&eventBefore,&event,sizeof(event)))return 1;++scenarios;CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(0);
    }
    printf("CLOSE_PHOTO_CAPTURE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
