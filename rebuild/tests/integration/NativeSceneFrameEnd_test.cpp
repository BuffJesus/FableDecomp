#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scene_frame_end_aliases.h"
#include "scene_frame_end_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall EndScene(void*,void*);
static unsigned char *arena,expected[16384],initial[16384];
struct Event{unsigned kind,self,index;unsigned char state[16384];};static Event events[20];
static unsigned mode,total,cursor,cases,snapshots,ids[9],maps,shape;
static void* primitiveTable[2];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
static unsigned Off(unsigned p){return p-(unsigned)arena;}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u\n",__LINE__,cases,cursor);exit(70);}}while(0)
static unsigned Build(unsigned first,unsigned count,unsigned stride,unsigned parent){
    if(!count)return 0;unsigned pivot=shape==0?count/2:shape==1?0:count-1,node=first+pivot*stride;
    W(arena,node)=1;W(arena,node+4)=Ptr(parent);W(arena,node+8)=Build(first,pivot,stride,node);W(arena,node+12)=Build(node+stride,count-pivot-1,stride,node);return Ptr(node);
}
static void Tree(unsigned header,unsigned first,unsigned count,unsigned stride){W(arena,header)=0;W(arena,header+4)=Build(first,count,stride,header);W(arena,header+8)=Ptr(count?first:header);W(arena,header+12)=Ptr(count?first+(count-1)*stride:header);}
static void Mutate(unsigned char*a,unsigned kind,unsigned self,unsigned index){
    if(kind==0){++W(a,self+4);if(mode==1&&index+1<maps){unsigned next=ids[index+1];W(a,0x600+48*next+0x14)=Ptr(0x3000+64*next);}if(mode==2)W(a,0x24)=Ptr(0x2400);}
    else if(mode==3){W(a,0x24)=0;W(a,self+0x10)^=0xdeadbeef;}
}
static void Callback(unsigned kind,void*self){REQUIRE(cursor<total);Event&e=events[cursor];REQUIRE(kind==e.kind&&self==arena+e.self&&!memcmp(arena,e.state,16384));++snapshots;Mutate(arena,kind,e.self,e.index);++cursor;}
extern "C" void __fastcall UpdateMap(void*self,void*){Callback(0,self);}
extern "C" void __fastcall DeletePrimitive(void*self,void*){Callback(1,self);}
static void Add(unsigned kind,unsigned self,unsigned index){REQUIRE(total<20);Event&e=events[total++];e.kind=kind;e.self=self;e.index=index;memcpy(e.state,expected,16384);Mutate(expected,kind,self,index);}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<3;++i){unsigned target=retailFixups[i][1]==0xb948e0?(unsigned)(code+16):retailFixups[i][1]==0xb91980?(unsigned)(code+208):(unsigned)&UpdateMap;*(unsigned*)(code+retailFixups[i][0])=target-(unsigned)(code+retailFixups[i][0]+4);}
    primitiveTable[1]=(void*)&DeletePrimitive;
    for(unsigned columns=0;columns<4;++columns)for(unsigned pattern=0;pattern<4;++pattern)for(unsigned outer=0;outer<3;++outer)for(unsigned inner=0;inner<3;++inner)for(mode=0;mode<4;++mode)for(unsigned count=0;count<5;++count)for(unsigned refs=0;refs<4;++refs){
        for(i=0;i<16384;++i)arena[i]=(unsigned char)(i*7+cases*41);maps=0;shape=outer;Tree(0x100,0x200,columns,64);W(arena,4)=Ptr(0x100);W(arena,8)=columns;
        for(unsigned c=0;c<columns;++c){unsigned size=(pattern+c)%4,header=0x400+64*c;shape=inner;Tree(header,0x600+48*3*c,size,48);W(arena,0x210+64*c)=c-1;W(arena,0x214+64*c)=Ptr(header);W(arena,0x218+64*c)=size;for(unsigned j=0;j<size;++j){unsigned id=3*c+j;ids[maps++]=id;W(arena,0x610+48*id)=j-1;W(arena,0x614+48*id)=Ptr(0x1400+64*id);}}
        const unsigned refcounts[]={0,1,2,0xffffffff};
        for(unsigned list=0;list<2;++list)for(i=0;i<5;++i){unsigned node=0x2000+list*0x400+i*80;W(arena,node)=(unsigned)primitiveTable;W(arena,node+4)=refcounts[(refs+i)%4];unsigned length=list?3:count;W(arena,node+0x38)=i+1<length?Ptr(node+80):0;}
        W(arena,0x24)=count?Ptr(0x2000):0;memcpy(initial,arena,16384);memcpy(expected,arena,16384);total=0;
        for(i=0;i<maps;++i)Add(0,Off(W(expected,0x614+48*ids[i])),i);
        unsigned node=W(expected,0x24),index=0;
        while(node){unsigned self=Off(node),next=W(expected,self+0x38);--W(expected,self+4);W(expected,self+0x38)=W(expected,self+0x3c)=W(expected,self+0x40)=W(expected,self+0x44)=0;if(!W(expected,self+4))Add(1,self,index);node=next;++index;REQUIRE(index<=5);}
        W(expected,0x24)=0;
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,16384);cursor=0;typedef void(__fastcall*Fn)(void*,void*);(run?EndScene:(Fn)code)(arena,0);REQUIRE(cursor==total&&!memcmp(arena,expected,16384));++snapshots;}++cases;
    }
    printf("NATIVE_SCENE_FRAME_END_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
