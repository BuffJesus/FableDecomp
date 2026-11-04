#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frame_data_reset_debug_retail.h"
#include "frame_data_reset_debug_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall ResetDebugMemory(void*,void*);
static unsigned arena[4096],initial[4096],expected[4096],location;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void model(unsigned self){*(unsigned char*)(self+58)=127;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));const unsigned patterns[6]={0,1,2,0x7fffffff,0x80000000,0xffffffff};unsigned cases=0,snapshots=0;
for(unsigned prior=0;prior<256;++prior)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;unsigned self=ptr(0x100);memset(arena,fill?0xa9:0,sizeof(arena));*(unsigned char*)(self+58)=(unsigned char)prior;memcpy(initial,arena,sizeof(arena));model(self);memcpy(expected,arena,sizeof(arena));for(unsigned mode=0;mode<2;++mode){memcpy(arena,initial,sizeof(arena));(mode?ResetDebugMemory:((void(__fastcall*)(void*,void*))code))((void*)self,0);require(!memcmp(expected,arena,sizeof(arena)));++snapshots;}++cases;}

VirtualFree(code,0,0x8000);printf("NATIVE_FRAME_DATA_RESET_DEBUG_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
