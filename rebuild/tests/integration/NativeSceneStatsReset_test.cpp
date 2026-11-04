#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scene_stats_reset_aliases.h"
#include "scene_stats_reset_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall ResetStats(void*,void*);
static unsigned char *arena,expected[4096],initial[4096];
struct Event{unsigned node;unsigned char state[4096];};static Event events[15];
static unsigned mode,total,cursor,cases,snapshots,shape;
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
static unsigned Off(unsigned p){return p-(unsigned)arena;}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u\n",__LINE__,cases,cursor);exit(70);}}while(0)
static unsigned Build(unsigned first,unsigned count,unsigned parent){
    if(!count)return 0;unsigned pivot=shape==0?count/2:shape==1?0:count-1,node=first+pivot*48;
    W(arena,node)=1;W(arena,node+4)=Ptr(parent);W(arena,node+8)=Build(first,pivot,node);W(arena,node+12)=Build(node+48,count-pivot-1,node);return Ptr(node);
}
static void Mutate(unsigned char*a,unsigned node){memset(a+node,0xdb,48);if(mode==1)W(a,0x2c)=Ptr(0x140);if(mode==2)W(a,0x38)=W(a,0x3c)=W(a,0x40)=0x12345678;if(mode==3)W(a,0x30)=0xffffffff;}
extern "C" void __cdecl SceneStatsFree(void*node){REQUIRE(cursor<total);Event&e=events[cursor];REQUIRE(node==arena+e.node&&!memcmp(arena,e.state,4096));++snapshots;Mutate(arena,e.node);++cursor;}
static void Collect(unsigned first,unsigned count){if(!count)return;unsigned pivot=shape==0?count/2:shape==1?0:count-1,node=first+pivot*48;Collect(node+48,count-pivot-1);REQUIRE(total<15);Event&e=events[total++];e.node=node;memcpy(e.state,expected,4096);Mutate(expected,node);Collect(first,pivot);}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<2;++i){unsigned target=retailFixups[i][1]==0xb95ee0?(unsigned)(code+80):(unsigned)&SceneStatsFree;*(unsigned*)(code+retailFixups[i][0])=target-(unsigned)(code+retailFixups[i][0]+4);}
    for(unsigned count=0;count<16;++count)for(shape=0;shape<3;++shape)for(mode=0;mode<4;++mode)for(unsigned fill=0;fill<4;++fill){
        for(i=0;i<4096;++i)arena[i]=(unsigned char)(i*7+fill*41);W(arena,0x2c)=Ptr(0x100);W(arena,0x30)=count;
        W(arena,0x100)=0;W(arena,0x104)=Build(0x200,count,0x100);W(arena,0x108)=Ptr(count?0x200:0x100);W(arena,0x10c)=Ptr(count?0x200+48*(count-1):0x100);
        W(arena,0x140)=0;W(arena,0x144)=0;W(arena,0x148)=W(arena,0x14c)=Ptr(0x140);
        memcpy(initial,arena,4096);memcpy(expected,arena,4096);total=0;Collect(0x200,count);
        if(count){unsigned header=Off(W(expected,0x2c));W(expected,header+8)=Ptr(header);W(expected,header+4)=0;W(expected,header+12)=Ptr(header);W(expected,0x30)=0;}
        W(expected,0x38)=W(expected,0x3c)=W(expected,0x40)=0;
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,4096);cursor=0;typedef void(__fastcall*Fn)(void*,void*);(run?ResetStats:(Fn)code)(arena,0);REQUIRE(cursor==total&&!memcmp(arena,expected,4096));++snapshots;}++cases;
    }
    printf("NATIVE_SCENE_STATS_RESET_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
