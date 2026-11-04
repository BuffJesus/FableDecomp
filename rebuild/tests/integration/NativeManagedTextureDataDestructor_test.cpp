#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "managed_texture_destructor_retail.h"
#include "managed_texture_destructor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall ManagedDestructor(void*,void*);
extern "C" unsigned ManagedVTable[];
static unsigned arena[4096],initial[4096],expected[7][4096],captures[7][4096];
static unsigned captured,mutation,ownerOffset;
static void require(bool value){if(!value)abort();}
static void capture(){require(captured<7);memcpy(captures[captured++],arena,sizeof(arena));}
static void event(unsigned id,void* data){unsigned n=arena[0x2000/4]++;require(n<6);arena[0x2000/4+1+n*2]=id;arena[0x2000/4+2+n*2]=(unsigned)data;capture();}
extern "C" void __fastcall BaseDestructorBoundary(void* self,void*){require(self==arena+ownerOffset);event(3,self);if(mutation&4)memset(self,0xab,76);}
extern "C" void __fastcall DeleteDataBoundary(void* data){unsigned* owner=arena+ownerOffset;unsigned index=owner[10]?11:9;event(2,data);if(mutation&2){owner[index]=(unsigned)(arena+(index==11?0x580:0x5c0)/4);owner[index-1]^=0x100;}}
void __cdecl operator delete(void* data){unsigned* owner=arena+ownerOffset;bool first=arena[0x2000/4]==0;event(1,data);if(first&&(mutation&1))owner[11]=(unsigned)(arena+0x580/4);if(mutation&4)((unsigned char*)owner)[75]^=0x59;}
extern "C" void __fastcall UnusedBoundary(void*,void*){abort();}
static unsigned pattern(unsigned n){const unsigned values[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static void release(unsigned index){unsigned* owner=arena+ownerOffset;unsigned* info=(unsigned*)owner[index];if(info){--info[0];info=(unsigned*)owner[index];if(!info[0]){((void(__fastcall*)(void*))info[1])((void*)info[2]);operator delete((void*)owner[index]);}}owner[index-1]=0;owner[index]=0;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[2]={(unsigned)&operator delete,(unsigned)BaseDestructorBoundary};require(sizeof(fixups)/sizeof(*fixups)==4);for(unsigned f=0;f<4;++f){const Fixup& fix=fixups[f];require(fix.kind==20&&fix.target<2);*(unsigned*)(code+fix.site)=targets[fix.target]-(unsigned)(code+fix.site+4);}unsigned cases=0,snapshots=0;
for(unsigned vp=0;vp<2;++vp)for(unsigned tp=0;tp<2;++tp)for(unsigned alias=0;alias<2;++alias)for(unsigned vc=0;vc<8;++vc)for(unsigned tc=0;tc<8;++tc)for(mutation=0;mutation<8;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
 ownerOffset=(place?0x1800:0)/4+0x100/4;unsigned* owner=arena+ownerOffset;memset(arena,fill?0xa9:0,sizeof(arena));for(unsigned i=0;i<19;++i)owner[i]=pattern(vc+i);owner[0]=(unsigned)ManagedVTable;owner[10]=(unsigned)(arena+0x800/4);owner[8]=(unsigned)(arena+0x900/4);owner[11]=vp?(unsigned)(arena+0x500/4):0;owner[9]=tp?(unsigned)(arena+(alias?0x500:0x540)/4):0;owner[14]=pattern(tc);
 for(unsigned b=0;b<4;++b){unsigned* info=arena+(0x500+b*0x40)/4;info[0]=pattern(b==0?vc:tc);info[1]=(unsigned)DeleteDataBoundary;info[2]=pattern(vc+tc+b);}memset(arena+0x2000/4,0,64);memcpy(initial,arena,sizeof(arena));unsigned expectedCount=0;
 for(unsigned mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));captured=0;if(!mode){operator delete((void*)owner[14]);release(11);release(9);BaseDestructorBoundary(owner,0);}else(mode==1?((void(__fastcall*)(void*,void*))code):ManagedDestructor)(owner,0);capture();if(!mode){expectedCount=captured;memcpy(expected,captures,sizeof(expected));}else{require(captured==expectedCount);for(unsigned i=0;i<captured;++i){require(!memcmp(expected[i],captures[i],sizeof(arena)));++snapshots;}}}++cases;
}VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_DATA_DESTRUCTOR_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
