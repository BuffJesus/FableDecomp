#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texture_level_count_retail.h"
#include "texture_level_count_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned long __fastcall TextureLevels(void*,void*);
extern "C" void UnusedBoundary(){abort();}
static unsigned arena[4096],initial[4096],images[2][4096],vtable[16];static unsigned *owner,*interface1,*interface2;static unsigned mode,mutation,selected,value,events,snapshots;
static void require(bool b){if(!b)abort();}
static unsigned endpoint(unsigned i){const unsigned values[]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[i&7];}
static void snap(unsigned i){if(!mode)memcpy(images[i],arena,sizeof(arena));else{require(!memcmp(images[i],arena,sizeof(arena)));++snapshots;}}
static unsigned __stdcall levels(void*self){require(events==0&&self==(selected?(void*)interface2:(void*)interface1));if(mutation==1)owner[0]=(unsigned)(selected?interface1:interface2);if(mutation==2)owner[1]=0xfedcba98;if(mutation==3){owner[0]=0;owner[1]=0x12345678;}arena[0x2000/4]=selected;++events;snap(0);return endpoint(value);}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));memset(vtable,0,sizeof(vtable));vtable[13]=(unsigned)levels;unsigned cases=0;
for(value=0;value<8;++value)for(mutation=0;mutation<4;++mutation)for(selected=0;selected<2;++selected)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){memset(arena,fill?0xa9:0,sizeof(arena));owner=arena+(place?0x1800:0)/4+0x100/4;interface1=owner+0x200/4;interface2=owner+0x300/4;interface1[0]=interface2[0]=(unsigned)vtable;owner[0]=(unsigned)(selected?interface2:interface1);memcpy(initial,arena,sizeof(arena));for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));events=0;unsigned result=mode==0?levels((void*)owner[0]):mode==1?((unsigned(__fastcall*)(void*,void*))code)(owner,0):TextureLevels(owner,0);require(events==1&&result==endpoint(value));arena[0x2100/4]=result;snap(1);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_TEXTURE_LEVEL_COUNT_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
