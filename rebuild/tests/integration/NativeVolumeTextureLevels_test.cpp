#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_volume_types.h"
#include "volume_texture_levels_retail.h"
#include "volume_texture_levels_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned __fastcall VolumeTextureLevels(void*,void*);
static unsigned arena[4096],initial[4096],images[2][4096],mode,step,count,snapshots,location,profile,mutation,calls,pointee;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void snapshot(){if(!mode){require(count<2);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static unsigned __stdcall unlock(void* volume){require(!calls&&(unsigned)volume==pointee);++calls;word(ptr(0x2000),0)=calls;word(ptr(0x2000),4)=(unsigned)volume;if(mutation&1){word(ptr(0x100),4)=pattern(profile);word(ptr(0x100),0)^=0xffffffff;}if(mutation&2){word(pointee,0)=pattern(profile+1);word(pointee,4)=pattern(profile+2);}snapshot();return pattern(profile);}
static unsigned model(){return unlock((void*)word(ptr(0x100),4));}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned table[14]={0};table[13]=(unsigned)unlock;unsigned cases=0;
for(profile=0;profile<8;++profile)for(mutation=0;mutation<4;++mutation)for(unsigned object=0;object<2;++object)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;pointee=ptr(object?0x800:0x400);memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=pointee;word(pointee,0)=(unsigned)table;word(pointee,4)=pattern(profile+3);memset((void*)ptr(0x2000),0,28);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=calls=0;unsigned result;if(!mode)result=model();else result=(mode==1?((unsigned(__fastcall*)(void*,void*))code):VolumeTextureLevels)((void*)ptr(0x100),0);word(ptr(0x2000),8)=result;require(result==pattern(profile));snapshot();if(mode)require(step==count&&calls==1);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_LEVELS_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
