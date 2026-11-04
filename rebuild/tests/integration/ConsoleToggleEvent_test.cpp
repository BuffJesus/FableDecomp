#include "fable_world.h"
#include "fable_game.h"
#include "fable_player_manager.h"
#include "fable_console.h"
#include <stdio.h>
#include <string.h>
CMainGameComponent* CMainGameComponent::PMainGameComponent;
CCountedPointer<CConsole> g_FableConsolePClass_013CAA40;
struct CGameEventDispatchTestAccess{static void Set(CMainGameComponent* c,CPlayerManager* m){CMainGameComponent::PMainGameComponent=c;memcpy(&c->PPlayerManager,&m,4);}};
struct CPlayerLookupTestAccess{static void Set(CPlayerManager& m,long n){m.MainPlayer=n;}};
class CConsoleTestAccess{public:static void Set(CConsole& c,unsigned char active){memcpy(&c.Active,&active,1);}};
static CPlayerManager* manager;static CPlayer* player;static long number;static unsigned calls,errors,ctorCalls,resetCalls;static bool lazy;static unsigned char active;static CConsole* allocated;
static unsigned long componentBefore[24000];
CPlayer& CPlayerManager::GetPlayer(long n){if(this!=manager || n!=number || calls)++errors;++calls;return *player;}
void CPlayer::AddMode(EPlayerMode m,bool force){if(this!=player || active || m!=PLAYER_MODE_CONSOLE || force || calls!=1)++errors;++calls;}
void CPlayer::RemoveMode(EPlayerMode m){if(this!=player || !active || m!=PLAYER_MODE_CONSOLE || calls!=1)++errors;++calls;}
// Constructor is an explicit ABI boundary; do not instantiate console members.
extern "C" CConsole* __fastcall ConsoleCtorBoundary(CConsole* self){if(!lazy || calls!=1 || ctorCalls)++errors;++ctorCalls;allocated=self;memset(self,0x55,sizeof(CConsole));CConsoleTestAccess::Set(*self,active);return self;}
#pragma comment(linker,"/alternatename:??0CConsole@@QAE@XZ=@ConsoleCtorBoundary@4")
template<> void CCountedPointer<CConsole>::Reset(CConsole* p){if(this!=&g_FableConsolePClass_013CAA40 || !lazy || calls!=1 || ctorCalls!=1 || p!=allocated || Data)++errors;++resetCalls;Data=p;Info=0;}
int main(){unsigned scenarios=0;static const long numbers[8]={0,1,-1,127,-128,(long)0x80000000UL,0x7fffffff,19};
    for(unsigned pattern=0;pattern<4;++pattern)for(unsigned n=0;n<8;++n)for(unsigned state=0;state<2;++state)for(unsigned flag=0;flag<256;++flag){unsigned long componentStorage[24000],managerStorage[32],playerStorage[160],consoleStorage[48],worldStorage[104];memset(componentStorage,pattern*71,sizeof(componentStorage));memset(managerStorage,pattern*71,sizeof(managerStorage));memset(playerStorage,pattern*71,sizeof(playerStorage));memset(consoleStorage,pattern*71,sizeof(consoleStorage));memset(worldStorage,pattern*71,sizeof(worldStorage));CMainGameComponent* component=reinterpret_cast<CMainGameComponent*>(componentStorage+2);manager=reinterpret_cast<CPlayerManager*>(managerStorage+2);player=reinterpret_cast<CPlayer*>(playerStorage+2);CConsole* console=reinterpret_cast<CConsole*>(consoleStorage+2);CWorld* world=reinterpret_cast<CWorld*>(worldStorage+2);number=numbers[n];active=(unsigned char)flag;lazy=state!=0;CGameEventDispatchTestAccess::Set(component,manager);CPlayerLookupTestAccess::Set(*manager,number);CConsoleTestAccess::Set(*console,active);g_FableConsolePClass_013CAA40.Data=lazy?0:console;g_FableConsolePClass_013CAA40.Info=0;unsigned long mb[32],pb[160],cb[48],wb[104];memcpy(mb,managerStorage,sizeof(mb));memcpy(pb,playerStorage,sizeof(pb));memcpy(cb,consoleStorage,sizeof(cb));memcpy(wb,worldStorage,sizeof(wb));calls=errors=ctorCalls=resetCalls=0;allocated=0;
        memcpy(componentBefore,componentStorage,sizeof(componentBefore));world->EAConsoleToggleState(reinterpret_cast<const CGameEvent*>(0xdeadbeef));if(errors || calls!=2 || ctorCalls!=(lazy?1:0) || resetCalls!=(lazy?1:0) || g_FableConsolePClass_013CAA40.Data!=(lazy?allocated:console) || g_FableConsolePClass_013CAA40.Info || memcmp(mb,managerStorage,sizeof(mb)) || memcmp(pb,playerStorage,sizeof(pb)) || memcmp(cb,consoleStorage,sizeof(cb)) || memcmp(wb,worldStorage,sizeof(wb)) || memcmp(componentBefore,componentStorage,sizeof(componentBefore)))return 1;
        if(lazy)::operator delete(allocated);g_FableConsolePClass_013CAA40.Data=0;g_FableConsolePClass_013CAA40.Info=0;++scenarios;
    }
    printf("CONSOLE_TOGGLE_EVENT_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
