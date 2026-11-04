#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "managed_texture_uninitialise_retail.h"
#include "managed_texture_uninitialise_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall ManagedUninitialise(void*,void*);
static unsigned arena[4096],initial[4096],expected[5][4096],captures[5][4096],captured,mutation,ownerOffset;
static void require(bool b){if(!b)abort();}
static void capture(){require(captured<5);memcpy(captures[captured++],arena,sizeof(arena));}
static void event(unsigned id,void* data){unsigned n=arena[0x2000/4]++;require(n<4);arena[0x2000/4+1+n*2]=id;arena[0x2000/4+2+n*2]=(unsigned)data;capture();}
extern "C" void __fastcall DeleteDataBoundary(void* data){unsigned* owner=arena+ownerOffset;event(1,data);if(mutation&1)owner[9]=(unsigned)(arena+0x580/4);if(mutation&2)owner[8]^=0x100;if(mutation&4){owner[13]=0xffffffff;owner[15]=0x80000000;owner[16]=7;}}
void __cdecl operator delete(void* data){unsigned* owner=arena+ownerOffset;event(2,data);if(mutation&2)owner[9]=(unsigned)(arena+0x5c0/4);if(mutation&4){owner[11]^=0x80;owner[17]^=0x100;}}
extern "C" void __fastcall TextureShutdownBoundary(void* self,void*){require(self==0);unsigned* owner=arena+ownerOffset;require(!owner[8]&&!owner[9]&&!owner[13]&&!owner[15]&&!owner[16]);event(3,self);if(mutation&4)((unsigned char*)owner)[75]^=0x59;}
static unsigned pattern(unsigned n){const unsigned values[]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static void model(){unsigned* owner=arena+ownerOffset;unsigned* info=(unsigned*)owner[9];if(info){--info[0];info=(unsigned*)owner[9];if(!info[0]){((void(__fastcall*)(void*))info[1])((void*)info[2]);operator delete((void*)owner[9]);}}owner[9]=0;owner[8]=0;owner[13]=0;owner[15]=0;owner[16]=0;TextureShutdownBoundary(0,0);}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[]={(unsigned)&operator delete,(unsigned)TextureShutdownBoundary};require(sizeof(fixups)/sizeof(*fixups)==2);for(unsigned f=0;f<2;++f){require(fixups[f].kind==20&&fixups[f].target<2);*(unsigned*)(code+fixups[f].site)=targets[fixups[f].target]-(unsigned)(code+fixups[f].site+4);}unsigned cases=0,snapshots=0;
 for(unsigned block=0;block<3;++block)for(unsigned ref=0;ref<8;++ref)for(mutation=0;mutation<8;++mutation)for(unsigned alias=0;alias<2;++alias)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){memset(arena,fill?0xa9:0,sizeof(arena));ownerOffset=((place?0x1800:0)+0x100)/4;unsigned* owner=arena+ownerOffset;for(unsigned i=0;i<19;++i)owner[i]=pattern(i+ref);owner[8]=alias?0xffffffff:(unsigned)(arena+0x900/4);owner[9]=block?(unsigned)(arena+(block==1?0x500:0x540)/4):0;for(unsigned b=0;b<4;++b){unsigned* info=arena+(0x500+b*0x40)/4;info[0]=pattern(ref);info[1]=(unsigned)DeleteDataBoundary;info[2]=pattern(ref+b);}memset(arena+0x2000/4,0,64);memcpy(initial,arena,sizeof(arena));unsigned expectedCount=0;
  for(unsigned mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));captured=0;if(!mode)model();else(mode==1?((void(__fastcall*)(void*,void*))code):ManagedUninitialise)(owner,0);capture();if(!mode){expectedCount=captured;memcpy(expected,captures,sizeof(expected));}else{require(captured==expectedCount);for(unsigned i=0;i<captured;++i){require(!memcmp(expected[i],captures[i],sizeof(arena)));++snapshots;}}}++cases;}
 VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_DATA_UNINITIALISE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
