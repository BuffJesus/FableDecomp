#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manager_uninitialise_retail.h"
#include "manager_uninitialise_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall ShutdownRoot(void*,void*);
static unsigned arena[4096],initial[4096],expected[4096];
static void require(bool b){if(!b)abort();}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));const unsigned values[]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};const unsigned char flags[]={0,1,0x80,0xff};unsigned cases=0,snapshots=0;
 for(unsigned f=0;f<4;++f)for(unsigned buffer=0;buffer<8;++buffer)for(unsigned pools=0;pools<8;++pools)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){memset(arena,fill?0xa9:0,sizeof(arena));unsigned offset=(place?0x1800:0)+0x100;unsigned char* manager=(unsigned char*)arena+offset;manager[4]=flags[f];*(unsigned*)(manager+0x5d0)=values[buffer];*(unsigned*)(manager+8)=values[pools];memcpy(initial,arena,sizeof(arena));memcpy(expected,initial,sizeof(arena));((unsigned char*)expected)[offset+4]=0;*(unsigned*)((unsigned char*)expected+offset+0x5d0)=0;
 for(unsigned mode=0;mode<2;++mode){memcpy(arena,initial,sizeof(arena));for(unsigned repeat=0;repeat<2;++repeat){if(mode)ShutdownRoot(manager,0);else ((void(__fastcall*)(void*,void*))code)(manager,0);require(!memcmp(expected,arena,sizeof(arena)));++snapshots;}}++cases;}
 VirtualFree(code,0,0x8000);printf("NATIVE_TEXTURE_MANAGER_UNINITIALISE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
