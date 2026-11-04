#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_definition_manager.h"
#include "fable_player_gui.h"
#include "player_gui_def_retail.h"
#include "player_gui_def_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned __fastcall GuiPeek();
extern "C" void AllocBoundary();
extern "C" void UnassignBoundary();
extern "C" void LookupBoundary();
extern "C" {unsigned ManagerSlot[2]={0,0};}
struct CPlayerGuiHeroInfoTestAccess{static unsigned& Cache(){return *(unsigned*)&CPlayerGui::PGuiDef;}};
#define GuiCache CPlayerGuiHeroInfoTestAccess::Cache()
fable_i32 g_CCharStringInstanceCount_013BD800=0;
static unsigned arena[16384], expected[16384];
struct State {unsigned cache,manager;long strings;};
static State states[16];static unsigned images[16][16384];
static unsigned count,step,mode,selected,overwrite,deleteMutation,dtorMutation,location,snapshots;
static unsigned ptr(unsigned offset){return (unsigned)((unsigned char*)arena+location+offset);}
static unsigned& word(unsigned p,unsigned off){return *(unsigned*)(p+off);}
static void require(bool b){if(!b){printf("failure mode=%u step=%u selected=%u overwrite=%u\n",mode,step,selected,overwrite);abort();}}
static void snapshot(){
 if(mode==0){require(count<16);memcpy(images[count],arena,sizeof(arena));states[count].cache=GuiCache;states[count].manager=ManagerSlot[0];states[count].strings=g_CCharStringInstanceCount_013BD800;++count;}
 else {require(step<count);require(!memcmp(images[step],arena,sizeof(arena)));require(states[step].cache==GuiCache&&states[step].manager==ManagerSlot[0]&&states[step].strings==g_CCharStringInstanceCount_013BD800);++step;++snapshots;}
}
class DeleteBoundary {public:void Delete() const;};
void DeleteBoundary::Delete() const {word((unsigned)this,8)=0xdededede;if(deleteMutation)GuiCache=ptr(0x800);snapshot();}
static void release(unsigned p){if(p&&--word(p,4)==0){word(p,8)=0xdededede;if(deleteMutation)GuiCache=ptr(0x800);snapshot();}}
CCharStringData* CCharString::AllocStringData(const char* text,long length){
 require(!strcmp(text,"PLAYER_GUI_PC")&&length==-1);CCharStringData* d=(CCharStringData*)ptr(0x1000);d->text=(char*)ptr(0x1100);d->unknown04=13;d->unknown08=14;d->flags0C=0;d->owners=1;memcpy(d->text,text,14);snapshot();return d;
}
void CCharString::UnassignString(){if(storage_){--storage_->owners;storage_=0;}if(dtorMutation)GuiCache=ptr(0x800);snapshot();}
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromInstantiationName(const CCharString& name) const {
 require((unsigned)this==ptr(0x2000));CCharStringData* data=*(CCharStringData*const*)&name;require(data==(CCharStringData*)ptr(0x1000)&&!strcmp(data->text,"PLAYER_GUI_PC"));unsigned p=selected?ptr(selected==1?0x400:0x800):0;if(p)++word(p,4);GuiCache=overwrite?ptr(overwrite==1?0x400:0x800):0;ManagerSlot[0]=ptr(0x2400);snapshot();return CDefPointer<const CDefClassBase>((const CDefClassBase*)p);
}
static void model(){if(!GuiCache){
 ++g_CCharStringInstanceCount_013BD800;CCharStringData* d=(CCharStringData*)ptr(0x1000);d->text=(char*)ptr(0x1100);d->unknown04=13;d->unknown08=14;d->flags0C=0;d->owners=1;memcpy(d->text,"PLAYER_GUI_PC",14);snapshot();
 unsigned p=selected?ptr(selected==1?0x400:0x800):0;if(p)++word(p,4);GuiCache=overwrite?ptr(overwrite==1?0x400:0x800):0;ManagerSlot[0]=ptr(0x2400);snapshot();
 if(p){if(GuiCache!=p){release(GuiCache);GuiCache=0;GuiCache=p;++word(p,4);}release(p);}
 --d->owners;if(dtorMutation)GuiCache=ptr(0x800);snapshot();--g_CCharStringInstanceCount_013BD800;
 }snapshot();}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 void (DeleteBoundary::*del)() const=&DeleteBoundary::Delete;unsigned deleteAddress=0;memcpy(&deleteAddress,&del,4);unsigned vtable[2]={0,deleteAddress};
 for(unsigned i=0;i<sizeof(fixups)/sizeof(fixups[0]);++i){unsigned t=fixups[i].target;unsigned value=t<sizeof(retailCode)?(unsigned)(code+t):t==0xffffffff?(unsigned)&GuiCache:t==0xfffffffe?(unsigned)&ManagerSlot:t==0xfffffffd?(unsigned)&g_CCharStringInstanceCount_013BD800:t==0xfffffffc?(unsigned)AllocBoundary:t==0xfffffffb?(unsigned)UnassignBoundary:(unsigned)LookupBoundary;*(unsigned*)(code+fixups[i].site)=fixups[i].kind==20?value-(unsigned)(code+fixups[i].site+4):value;}
 unsigned cases=0;const unsigned refs[4]={0,1,2,0xffffffff};
 for(unsigned cache=0;cache<3;++cache)for(selected=0;selected<3;++selected)for(overwrite=0;overwrite<3;++overwrite)for(deleteMutation=0;deleteMutation<2;++deleteMutation)for(dtorMutation=0;dtorMutation<2;++dtorMutation)for(unsigned r=0;r<4;++r)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
 location=place?0x6000:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x400),0)=(unsigned)vtable;word(ptr(0x800),0)=(unsigned)vtable;word(ptr(0x400),4)=refs[r];word(ptr(0x800),4)=refs[(r+1)%4];memcpy(expected,arena,sizeof(arena));count=0;
 unsigned result=0;for(mode=0;mode<3;++mode){memcpy(arena,expected,sizeof(arena));GuiCache=cache?ptr(cache==1?0x400:0x800):0;ManagerSlot[0]=ptr(0x2000);g_CCharStringInstanceCount_013BD800=17;step=0;if(mode==0){model();result=GuiCache;}else {unsigned actual=(mode==1?((unsigned(__fastcall*)())code)():GuiPeek());snapshot();require(actual==result&&step==count);}}
 ++cases;
 }GuiCache=0;VirtualFree(code,0,0x8000);printf("NATIVE_PLAYER_GUI_DEF_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
