#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_d3d_format_info.h"
#include "pixel_format_channel_access_retail.h"
#include "pixel_format_channel_access_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned long __fastcall Channel0(void*,void*);
extern "C" unsigned long __fastcall Channel1(void*,void*);
extern "C" unsigned long __fastcall Channel2(void*,void*);
extern "C" unsigned long __fastcall Channel3(void*,void*);
extern "C" unsigned long __fastcall Channel4(void*,void*);
static unsigned arena[4096],initial[4096],image[4096];
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));require(sizeof(fixups)/sizeof(*fixups)==4);for(unsigned i=0;i<4;++i){require(fixups[i].kind==6);*(unsigned*)(code+fixups[i].site)=(unsigned)GD3DFormats+(fixups[i].target&0x7fffffff);}require(!memcmp(GD3DFormats,retailFormats,sizeof(retailFormats)));unsigned cases=0,snapshots=0;
typedef unsigned long(__fastcall *Root)(void*,void*);Root roots[]={Channel0,Channel1,Channel2,Channel3,Channel4};const unsigned slots[]={6,0,4,5,6};const unsigned offsets[]={0,16,32,48,64};
for(unsigned op=0;op<5;++op)for(unsigned row=0;row<47;++row)for(unsigned profile=0;profile<8;++profile)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){memset(arena,fill?0xa9:0,sizeof(arena));unsigned* owner=arena+(place?0x1800:0)/4+0x100/4;owner[-1]=pattern(profile);owner[0]=row;owner[1]=pattern(profile+1);memcpy(initial,arena,sizeof(arena));for(unsigned mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));unsigned expected=op==1?0:retailFormats[row*9+slots[op]];unsigned result=mode==0?expected:mode==1?((Root)(code+offsets[op]))(owner,0):roots[op](owner,0);require(result==expected&&!memcmp(initial,arena,sizeof(arena))&&!memcmp(GD3DFormats,retailFormats,sizeof(retailFormats)));arena[0x2000/4]=result;if(!mode)memcpy(image,arena,sizeof(arena));else{require(!memcmp(image,arena,sizeof(arena)));++snapshots;}}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_PIXEL_FORMAT_CHANNEL_ACCESS_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
