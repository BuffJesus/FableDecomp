#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_world.h"
#include "fable_thing_manager.h"
#include "tc_manager_accessors_retail.h"
#include "tc_manager_accessors_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned __fastcall TCGetSeed(void*,void*);
extern "C" unsigned __fastcall TCGetDefinitions(void*,void*);
extern "C" unsigned __fastcall TCGetWorld(void*,void*);
extern "C" unsigned __fastcall TCPeekWorld(void*,void*);
extern "C" unsigned __fastcall TCGetPlayers(void*,void*);
extern "C" unsigned __fastcall TCPeekPlayers(void*,void*);
extern "C" unsigned __fastcall TCPeekComponent(void*,void*);
extern "C" {extern void* CurrentThingSlot;}
static void require(bool b){if(!b)abort();}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,160,0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+84)=(unsigned)(code+128)-(unsigned)(code+88);*(void**)(code+129)=&CurrentThingSlot;*(void**)(code+97)=&CurrentThingSlot;for(unsigned i=0;i<5;++i)*(void**)(code+i*16+1)=&CurrentThingSlot;
 typedef unsigned(__fastcall* Getter)(void*,void*);Getter getters[7]={TCGetWorld,TCPeekWorld,TCGetPlayers,TCPeekPlayers,TCPeekComponent,TCGetDefinitions,TCGetSeed};const unsigned offsets[7]={0x24,0x24,0x30,0x30,0x1c,0x20,0x1c};
 unsigned storage[4096],before[4096];unsigned cases=0;
 for(unsigned pattern=0;pattern<256;++pattern)for(unsigned kind=0;kind<7;++kind)for(unsigned location=0;location<2;++location)for(unsigned fill=0;fill<2;++fill)for(unsigned self=0;self<2;++self){
  memset(storage,fill?0xa9:0,sizeof(storage));unsigned char* p=(unsigned char*)storage+(location?0x1100:0x100);unsigned value=pattern==0?0:pattern==1?0xffffffff:pattern==2?0x80000000:pattern*0x13579bdf;
  *(unsigned*)(p+0x1c)=value^0x12345678;*(unsigned*)(p+0x24)=~value;*(unsigned*)(p+0x30)=value;*(unsigned*)(p+offsets[kind])=value;
  *(unsigned*)((unsigned char*)storage+0x3004)=self?0xabcdef00:0;CThing::SetCurrentThingManager(*(CThingManager*)p);require(CurrentThingSlot==p);memcpy(before,storage,sizeof(storage));
  for(unsigned run=0;run<2;++run){unsigned result=(run?getters[kind]:(Getter)(code+kind*16))(kind==5||(self!=0)?(unsigned char*)storage+0x3000:0,0);require(result==(kind==6?value+0x16140:value));require(CurrentThingSlot==p);require(!memcmp(storage,before,sizeof(storage)));}
  ++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_TC_MANAGER_ACCESSORS_PASS cases=%u snapshots=%u\n",cases,cases*2);return 0;
}
