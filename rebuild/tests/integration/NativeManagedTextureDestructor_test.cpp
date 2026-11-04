#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "managed_texture_destructor_retail.h"
#include "managed_texture_destructor_aliases.h"
#pragma comment(linker,"/alternatename:?ReleaseTexture@CManagedTextureData@@QAEXXZ=?TextureRelease@Boundaries@@QAEXXZ")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall ManagedDestructor(void*,void*);
extern "C" unsigned ManagedVTable[],BaseVTable[];
extern "C" void AbortBoundary(){abort();}
static unsigned arena[4096],initial[4096],images[3][4096],mode,steps,count,snapshots,location,notifyMutation,deleteMutation,notifyCalls,deleteCalls;
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned p,unsigned off){return *(unsigned*)(p+off);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(mode==0){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(steps<count&&!memcmp(images[steps++],arena,sizeof(arena)));++snapshots;}}
static void notify(unsigned object,unsigned handle){require(object==ptr(0x400)&&handle==ptr(0x100));++notifyCalls;word(ptr(0x300),0)=notifyCalls;if(notifyMutation==1)word(handle,4)=0;if(notifyMutation==2)word(handle,4)=ptr(0x800);if(notifyMutation==3)word(object,4)=0;snapshot();}
static void destroy(unsigned object){require(object==ptr(0x400)||object==ptr(0x800));++deleteCalls;word(ptr(0x300),4)=deleteCalls;word(object,8)=0xdededede;if(deleteMutation)word(ptr(0x100),4)=ptr(0x800);snapshot();}
class Boundaries {public:void TextureRelease();void Delete()const;};
void Boundaries::TextureRelease(){notify((unsigned)this,ptr(0x100));}
void Boundaries::Delete()const{destroy((unsigned)this);}
static void model(){unsigned h=ptr(0x100),p=word(h,4);word(h,0)=(unsigned)ManagedVTable;
if(p&&word(p,4)==2){notify(p,h);p=word(h,4);if(p){if(--word(p,4)==0)destroy(p);word(h,4)=0;}word(h,4)=0;}
p=word(h,4);if(p){if(--word(p,4)==0)destroy(p);word(h,4)=0;}word(h,0)=(unsigned)BaseVTable;snapshot();}

int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 void(Boundaries::*d)()const=&Boundaries::Delete;void(Boundaries::*n)()=&Boundaries::TextureRelease;unsigned table[4]={0,0,0,0};memcpy(&table[1],&d,4);memcpy(&table[3],&n,4);for(unsigned i=0;i<sizeof(fixups)/sizeof(*fixups);++i){const Fixup& f=fixups[i];unsigned target=f.target&0x80000000?(f.target==0x80000000?(unsigned)ManagedVTable:f.target==0x80000001?(unsigned)BaseVTable:table[3]):(unsigned)(code+f.target);*(unsigned*)(code+f.site)=f.kind==20?target-(unsigned)(code+f.site+4):target;}const unsigned refs[6]={0,1,2,3,0x80000000,0xffffffff};unsigned cases=0;
 for(unsigned live=0;live<2;++live)for(unsigned ref=0;ref<6;++ref)for(unsigned altRef=0;altRef<3;++altRef)for(notifyMutation=0;notifyMutation<4;++notifyMutation)for(deleteMutation=0;deleteMutation<2;++deleteMutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
 location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),4)=live?ptr(0x400):0;word(ptr(0x400),0)=word(ptr(0x800),0)=(unsigned)table;word(ptr(0x400),4)=refs[ref];word(ptr(0x800),4)=refs[altRef];word(ptr(0x300),0)=word(ptr(0x300),4)=0;memcpy(initial,arena,sizeof(arena));count=0;
 unsigned expectedNotify=0,expectedDelete=0;bool expectedReturn=false;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));steps=notifyCalls=deleteCalls=0;if(mode==0){model();expectedNotify=notifyCalls;expectedDelete=deleteCalls;}else{(mode==1?((void(__fastcall*)(void*,void*))code):ManagedDestructor)((void*)ptr(0x100),0);snapshot();require(steps==count&&notifyCalls==expectedNotify&&deleteCalls==expectedDelete);}}
 ++cases;
 }VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_DESTRUCTOR_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
