#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_thing_components.h"
#include "hero_drunkenness_retail.h"
#include "hero_drunkenness_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall HeroDraw(void*,void*,const void*);
extern "C" { void* MainSlot=0; }
static unsigned arena[4096],initial[4096],expected[4096];
static unsigned calls,deletes,mutation;static void* wantedStats;static void* wantedEngine;static void* expired;
static void require(bool b){if(!b)abort();}
void __cdecl operator delete(void* p){if(p==expired){++deletes;return;}free(p);}
void CTCHeroStats::RenderHeroDrunkenness(CIEngine* engine){
 require(this==wantedStats&&engine==wantedEngine);++calls;
 if(mutation){arena[4000]^=0x13579bdf;MainSlot=(char*)arena+0x3800;}
}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(fixups)/sizeof(fixups[0]);++i){const Fixup& f=fixups[i];unsigned target=f.target==0xffffffff?(unsigned)&MainSlot:f.target==0xfffffffe?(unsigned)DrawBoundary:f.target==0xfffffffd?(unsigned)DeleteBoundary:(unsigned)(code+f.target);*(unsigned*)(code+f.site)=f.kind==6?target:target-(unsigned)(code+f.site+4);}
 unsigned cases=0;
 for(unsigned state=0;state<4;++state)for(unsigned map=0;map<7;++map)for(unsigned bit=0;bit<2;++bit)for(unsigned count=0;count<3;++count)for(unsigned pos=0;pos<2;++pos)for(mutation=0;mutation<2;++mutation){
  memset(arena,pos?0xa9:0,sizeof(arena));unsigned char* a=(unsigned char*)arena;unsigned off=pos?0x100:0;
  unsigned char *display=a+off,*main=a+0x400,*manager=a+0x500,*player=a+0x800,*hero=a+0xc00,*data=a+0xe00,*entries=a+0x1000;
  unsigned* players=(unsigned*)(a+0x1400);players[0]=(unsigned)(a+0x1800);players[1]=(unsigned)player;players[2]=(unsigned)(a+0x1c00);
  *(unsigned*)(a+0x1800+0x28)=7;*(unsigned*)(player+0x28)=42;*(unsigned*)(a+0x1c00+0x28)=42;
  *(void**)(display+0x2c)=a+0x2000;*(void**)(main+0x1c)=manager;
  *(void**)(manager+0xc)=players;*(void**)(manager+0x10)=players+3;*(void**)(manager+0x14)=players+3;*(unsigned*)(manager+0x18)=1;*(unsigned*)(manager+0x1c)=count==0?42:count==1?99:7;
  unsigned char* selected=count==2?a+0x1800:player;
  *(void**)(selected+0x30)=state?data:0;*(void**)data=state==1?hero:0;*(unsigned*)(data+4)=state==2?1:2;
  *(unsigned*)(hero+0x20)=bit?0x10:0;*(void**)(hero+0x44)=entries;
  unsigned n=map==0?0:map==1?1:3;*(void**)(hero+0x48)=entries+n*8;*(void**)(hero+0x4c)=entries+4*8;
  unsigned keys[4]={1,4,9,0};if(map==1)keys[0]=4;if(map==2)keys[1]=3;if(map==3)keys[1]=5;if(map==4)keys[0]=4;if(map==5)keys[2]=4;
  for(unsigned j=0;j<n;++j){*(unsigned*)(entries+j*8)=keys[j];*(void**)(entries+j*8+4)=map==6?0:a+0x2400+j*64;}
  *(void**)(entries+n*8+4)=0;memcpy(initial,arena,sizeof(arena));memcpy(expected,arena,sizeof(arena));
  unsigned draw=0,index=0;while(index<n&&keys[index]<4)++index;
  if(state==1&&bit&&index<n&&keys[index]==4&&map!=6)draw=1;
  if(state>=2){*(void**)((unsigned char*)expected+(selected-a)+0x30)=0;--*(unsigned*)((unsigned char*)expected+0xe04);}
  if(draw&&mutation)expected[4000]^=0x13579bdf;
  for(unsigned run=0;run<2;++run){
   memcpy(arena,initial,sizeof(arena));MainSlot=main;calls=deletes=0;expired=data;wantedStats=a+0x2400+index*64;wantedEngine=a+0x2000;
   if(run)HeroDraw(display,0,a+0x2800);else ((void(__fastcall*)(void*,void*,const void*))code)(display,0,a+0x2800);
   require(calls==draw&&deletes==(state==2?1u:0u));require(!memcmp(arena,expected,sizeof(arena)));require(MainSlot==(draw&&mutation?a+0x3800:main));
  }
  ++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_HERO_DRUNKENNESS_PASS cases=%u snapshots=%u\n",cases,cases*2);return 0;
}
