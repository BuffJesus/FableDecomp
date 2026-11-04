#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resource_unused_frames_retail.h"
#include "resource_unused_frames_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned long __fastcall UnusedFrames(void*,void*);
static unsigned arena[4096],initial[4096],expected[4096],location;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned model(unsigned self){unsigned list=word(self,8);return list?word(list,44)-word(self,24):0xffffffff;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));const unsigned patterns[6]={0,1,2,0x7fffffff,0x80000000,0xffffffff};unsigned cases=0,snapshots=0;
for(unsigned live=0;live<2;++live)for(unsigned first=0;first<6;++first)for(unsigned second=0;second<6;++second)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;unsigned self=ptr(0x100);memset(arena,fill?0xa9:0,sizeof(arena));word(self,8)=live?ptr(0x400):0;word(self,24)=patterns[first];word(ptr(0x400),44)=patterns[second];memcpy(initial,arena,sizeof(arena));unsigned result=model(self);for(unsigned mode=0;mode<2;++mode){memcpy(arena,initial,sizeof(arena));unsigned actual=(mode?UnusedFrames:((unsigned long(__fastcall*)(void*,void*))code))((void*)self,0);require(actual==result&&!memcmp(initial,arena,sizeof(arena)));++snapshots;}++cases;}

VirtualFree(code,0,0x8000);printf("NATIVE_RESOURCE_UNUSED_FRAMES_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
