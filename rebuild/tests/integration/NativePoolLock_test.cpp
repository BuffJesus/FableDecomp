#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pool_lock_retail.h"
#include "pool_lock_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall PoolLock(void*,void*,long);
static unsigned arena[4096],initial[4096],image[4096],location;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void model(unsigned handle){unsigned header=word(word(ptr(0x100),0x438),handle*4);unsigned char& locked=*(unsigned char*)(header+15);if(!locked){locked=1;word(ptr(0x100),24)++;}}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));const unsigned lockedBytes[5]={0,1,2,0x80,0xff},handles[3]={0,1,7};unsigned cases=0,snapshots=0;
for(unsigned handle=0;handle<3;++handle)for(unsigned header=0;header<3;++header)for(unsigned locked=0;locked<5;++locked)for(unsigned counter=0;counter<8;++counter)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),24)=pattern(counter);word(ptr(0x100),0x438)=ptr(0x800);unsigned entry=ptr(header==0?0x400:header==1?0x600:0x109);word(ptr(0x800),handles[handle]*4)=entry;*(unsigned char*)(entry+15)=(unsigned char)lockedBytes[locked];memcpy(initial,arena,sizeof(arena));for(unsigned mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));if(!mode){model(handles[handle]);memcpy(image,arena,sizeof(arena));}else{(mode==1?((void(__fastcall*)(void*,void*,long))code):PoolLock)((void*)ptr(0x100),0,(long)handles[handle]);require(!memcmp(image,arena,sizeof(arena)));++snapshots;}}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_POOL_LOCK_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
