#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "volume_texture_constructor_retail.h"
#include "volume_texture_constructor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void* __fastcall VolumeTextureConstructor(void*,void*);
extern "C" unsigned VolumeVTable[],BaseVTable[];
extern "C" void* __fastcall VolumeTextureScalarDestructor(void*,void*,unsigned);
extern "C" void AbortBoundary(){abort();}
static unsigned arena[4096],initial[4096],images[2][4096],mode,step,count,snapshots,location,profile,mutation,calls,pointee,source;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void snapshot(){if(!mode){require(count<2);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static unsigned model(){word(ptr(0x100),0)=(unsigned)BaseVTable;word(ptr(0x100),4)=0;word(ptr(0x100),12)=0;word(ptr(0x100),0)=(unsigned)VolumeVTable;return ptr(0x100);}
int main(){require(VolumeVTable[0]==(unsigned)&VolumeTextureScalarDestructor);unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[2]={(unsigned)VolumeVTable,(unsigned)BaseVTable};for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=fixups[f].target>=0x80000000?targets[fixups[f].target-0x80000000]:(unsigned)(code+fixups[f].target);*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}unsigned cases=0;
for(profile=0;profile<8;++profile)for(unsigned length=0;length<8;++length)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=pattern(profile+1);word(ptr(0x100),8)=pattern(length);word(ptr(0x100),12)=pattern(profile+2);memset((void*)ptr(0x2000),0,28);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=0;unsigned result=!mode?model():(unsigned)(mode==1?((void*(__fastcall*)(void*,void*))code):VolumeTextureConstructor)((void*)ptr(0x100),0);word(ptr(0x2000),24)=result;snapshot();if(mode)require(step==count&&word(ptr(0x100),8)==pattern(length)&&result==ptr(0x100));}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_CONSTRUCTOR_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
