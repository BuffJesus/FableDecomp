#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manager_pool_access_retail.h"
#include "manager_pool_access_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned long __fastcall CountRoot(void*,void*);
extern "C" unsigned long __fastcall SizeRoot(void*,void*,unsigned long);
extern "C" unsigned long __fastcall TextureCountRoot(void*,void*,unsigned long);
static unsigned arena[4096],initial[4096];
static void require(bool b){if(!b)abort();}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));const unsigned values[]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};unsigned cases=0;
 for(unsigned value=0;value<8;++value)for(unsigned index=0;index<16;++index)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){memset(arena,fill?0xa9:0,sizeof(arena));unsigned char* manager=(unsigned char*)arena+(place?0x1800:0)+0x100;*(unsigned*)(manager+8)=values[value];for(unsigned i=0;i<16;++i){*(unsigned*)(manager+0xc+i*4)=values[(i+value)&7];*(unsigned*)(manager+0x88+i*0x54)=values[(i+value+3)&7];}memcpy(initial,arena,sizeof(arena));
 require(((unsigned(__fastcall*)(void*,void*))code)(manager,0)==values[value]);require(!memcmp(initial,arena,sizeof(arena)));require(CountRoot(manager,0)==values[value]);require(!memcmp(initial,arena,sizeof(arena)));
 require(((unsigned(__fastcall*)(void*,void*,unsigned))(code+16))(manager,0,index)==values[(index+value)&7]);require(!memcmp(initial,arena,sizeof(arena)));require(SizeRoot(manager,0,index)==values[(index+value)&7]);require(!memcmp(initial,arena,sizeof(arena)));
 require(((unsigned(__fastcall*)(void*,void*,unsigned))(code+32))(manager,0,index)==values[(index+value+3)&7]);require(!memcmp(initial,arena,sizeof(arena)));require(TextureCountRoot(manager,0,index)==values[(index+value+3)&7]);require(!memcmp(initial,arena,sizeof(arena)));++cases;}
 VirtualFree(code,0,0x8000);printf("NATIVE_TEXTURE_MANAGER_POOL_ACCESS_PASS cases=%u snapshots=%u\n",cases,cases*6);return 0;}
