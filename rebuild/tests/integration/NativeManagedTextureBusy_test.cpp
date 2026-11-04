#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "managed_texture_busy_retail.h"
#include "managed_texture_busy_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" bool __fastcall ManagedBusy(void*,void*);
extern "C" bool __fastcall TextureBusy(void*,void*);
static unsigned arena[4096],initial[4096],image[4096];
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));require(sizeof(fixups)/sizeof(*fixups)==1&&fixups[0].kind==20&&fixups[0].target==64);*(unsigned*)(code+fixups[0].site)=(unsigned)(code+64)-(unsigned)(code+fixups[0].site+4);const unsigned char busyValues[8]={0,1,2,3,0x7f,0x80,0xfe,0xff};unsigned cases=0,snapshots=0;
for(unsigned profile=0;profile<8;++profile)for(unsigned busy=0;busy<8;++busy)for(unsigned pd=0;pd<8;++pd)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){unsigned* owner=arena+(place?0x1800:0)/4+0x100/4;memset(arena,fill?0xa9:0,sizeof(arena));for(unsigned i=0;i<19;++i)owner[i]=pattern(profile+i);owner[8]=pattern(pd);((unsigned char*)owner)[0x4a]=busyValues[busy];memcpy(initial,arena,sizeof(arena));for(unsigned mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));bool result;if(!mode){((unsigned char*)owner)[0x4a]=0;result=false;}else result=(mode==1?((bool(__fastcall*)(void*,void*))code):ManagedBusy)(owner,0);require(!result&&((unsigned char*)owner)[0x4a]==0);bool leafResult=mode==0?false:mode==1?((bool(__fastcall*)(void*,void*))(code+64))((void*)pattern(pd),0):TextureBusy((void*)pattern(pd),0);require(!leafResult);arena[0x2000/4]=result;arena[0x2000/4+1]=leafResult;if(!mode)memcpy(image,arena,sizeof(arena));else{require(!memcmp(image,arena,sizeof(arena)));++snapshots;}}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_BUSY_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
