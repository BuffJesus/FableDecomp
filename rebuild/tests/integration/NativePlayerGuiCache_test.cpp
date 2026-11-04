#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_player_gui.h"
#include "player_gui_cache_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
typedef void(__cdecl* Cleanup)();
struct CPlayerGuiHeroInfoTestAccess{static unsigned& Cache(){return *(unsigned*)&CPlayerGui::PGuiDef;}};
static Cleanup registered;static unsigned registrations;
extern "C" int __cdecl atexit(Cleanup callback){const unsigned char* p=(const unsigned char*)callback;if(p[0]==0xa1&&*(const unsigned*)(p+1)==(unsigned)&CPlayerGuiHeroInfoTestAccess::Cache()){registered=callback;++registrations;}return 0;}
static unsigned memory[4096],before[4096],expected[4096],mutation,mode,callbacks;
static void require(bool b){if(!b){fprintf(stderr,"failure registrations=%u mode=%u callbacks=%u cache=%x\n",registrations,mode,callbacks,CPlayerGuiHeroInfoTestAccess::Cache());fflush(stderr);abort();}}
class DeleteBoundary{public:void Delete()const;};
void DeleteBoundary::Delete()const{*(unsigned*)((unsigned)this+8)=0xdededede;if(mutation)CPlayerGuiHeroInfoTestAccess::Cache()=(unsigned)memory+0x800;++callbacks;}
int main(){
 require(registrations==1&&registered!=0&&CPlayerGuiHeroInfoTestAccess::Cache()==0);Cleanup built=registered;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 *(unsigned*)(code+1)=(unsigned)(code+32);*(unsigned*)(code+6)=(unsigned)atexit-(unsigned)(code+10);*(unsigned*)(code+33)=(unsigned)&CPlayerGuiHeroInfoTestAccess::Cache();*(unsigned*)(code+63)=(unsigned)&CPlayerGuiHeroInfoTestAccess::Cache();
 ((Cleanup)code)();require(registrations==2&&registered==(Cleanup)(code+32));
 void(DeleteBoundary::*del)()const=&DeleteBoundary::Delete;unsigned address=0;memcpy(&address,&del,4);unsigned table[2]={0,address};const unsigned refs[5]={0,1,2,0x80000000,0xffffffff};unsigned cases=0;
 for(unsigned live=0;live<2;++live)for(unsigned r=0;r<5;++r)for(mutation=0;mutation<2;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
 memset(memory,fill?0xa9:0,sizeof(memory));unsigned p=(unsigned)memory+(place?0x1000:0x400);*(unsigned*)p=(unsigned)table;*(unsigned*)(p+4)=refs[r];memcpy(before,memory,sizeof(memory));memcpy(expected,memory,sizeof(memory));unsigned wantCallbacks=live&&refs[r]==1?1:0;if(live){--*(unsigned*)((unsigned)expected+p-(unsigned)memory+4);if(wantCallbacks)*(unsigned*)((unsigned)expected+p-(unsigned)memory+8)=0xdededede;}
 for(mode=0;mode<2;++mode){memcpy(memory,before,sizeof(memory));CPlayerGuiHeroInfoTestAccess::Cache()=live?p:0;callbacks=0;(mode?built:registered)();require(callbacks==wantCallbacks&&CPlayerGuiHeroInfoTestAccess::Cache()==0&&!memcmp(memory,expected,sizeof(memory)));}
 ++cases;
 }VirtualFree(code,0,0x8000);printf("NATIVE_PLAYER_GUI_CACHE_PASS cases=%u snapshots=%u\n",cases,cases*2);fflush(stdout);return 0;
}
