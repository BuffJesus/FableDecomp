#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine_layer_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall RenderLayer(void*,void*,const void*);
typedef void(__fastcall*Fn)(void*,void*,const void*);
static unsigned char arena[1024],expected[1024],initial[1024];
static unsigned cases,snapshots,events,cursor,mode,layerValue,flagValue;
struct Event{unsigned kind,self,result;unsigned char state[1024];};static Event logEntries[40];
static void* table[11];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
static unsigned Off(unsigned p){return p-(unsigned)arena;}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u\n",__LINE__,cases,cursor);exit(70);}}while(0)
static void Mutate(unsigned kind,unsigned self,unsigned char*a){
    if(kind==1&&mode==1){unsigned n=(W(a,20)-W(a,16))/4;W(a,16)=Ptr(80);W(a,20)=Ptr(80+n*4);}
    if(kind==1&&mode==2)W(a,20)=W(a,16)+4;
    if(kind==1&&mode==3)W(a,248)^=1;
    if(kind==2&&mode==4)a[self+8]^=1;
    if(kind==2&&mode==5){W(a,4)^=0x12345678;W(a,8)^=0xffffffff;W(a,12)=0;}
}
static unsigned ModelEvent(unsigned kind,unsigned self){REQUIRE(events<40);Event&e=logEntries[events++];e.kind=kind;e.self=self;e.result=kind==1?W(expected,self+4):0;memcpy(e.state,expected,1024);Mutate(kind,self,expected);return e.result;}
static unsigned Observe(unsigned kind,void*self,const void*arg){
    REQUIRE(cursor<events);Event&e=logEntries[cursor++];REQUIRE(kind==e.kind&&self==arena+e.self&&!memcmp(arena,e.state,1024));
    if(kind!=1){const unsigned*p=(const unsigned*)arg;REQUIRE(p&&p[0]==Ptr(128)&&p[1]==layerValue&&p[2]==flagValue);}
    ++snapshots;Mutate(kind,e.self,arena);return e.result;
}
extern "C" unsigned __fastcall Peek(void*self,void*){return Observe(1,self,0);}
extern "C" void __fastcall Pre(void*self,void*,const void*arg){Observe(2,self,arg);}
extern "C" void __fastcall Draw(void*self,void*,const void*arg){Observe(3,self,arg);}
extern "C" void __fastcall Post(void*self,void*,const void*arg){Observe(4,self,arg);}
static void Model(){
    events=0;if(!(W(expected,12)&W(expected,248)))return;
    for(unsigned phase=0;phase<3;++phase)
        for(unsigned i=0;i<(W(expected,20)-W(expected,16))/4;++i){unsigned pos=Off(W(expected,16))+i*4;unsigned mask=ModelEvent(1,Off(W(expected,pos)));if(mask&W(expected,248)){pos=Off(W(expected,16))+i*4;unsigned node=Off(W(expected,pos));if(phase!=1||expected[node+8])ModelEvent(2+phase,node);}}
}
int main(){
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    table[4]=(void*)&Draw;table[5]=(void*)&Pre;table[6]=(void*)&Post;table[10]=(void*)&Peek;
    const unsigned values[]={0,1,7,0x80000000};
    for(unsigned count=0;count<4;++count)for(unsigned filter=0;filter<4;++filter)for(unsigned outer=0;outer<3;++outer)
    for(unsigned enabled=0;enabled<4;++enabled)for(mode=0;mode<6;++mode)for(unsigned fill=0;fill<2;++fill){
        for(unsigned k=0;k<1024;++k)arena[k]=(unsigned char)(k*17+fill*83);
        layerValue=fill?0x80000000:4;flagValue=enabled?0xffffffff:0;
        W(arena,4)=layerValue;W(arena,8)=flagValue;W(arena,12)=values[outer];W(arena,248)=values[filter];W(arena,16)=Ptr(64);W(arena,20)=Ptr(64+count*4);W(arena,24)=Ptr(80);
        for(k=0;k<3;++k){unsigned off=512+16*k;W(arena,off)=(unsigned)table;W(arena,off+4)=1<<k;arena[off+8]=(unsigned char)(enabled==3?255:(enabled>>k)&1);W(arena,64+4*k)=Ptr(off);W(arena,80+4*k)=Ptr(512+16*((k+1)%3));}
        memcpy(initial,arena,1024);memcpy(expected,arena,1024);Model();
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,1024);cursor=0;(run?RenderLayer:(Fn)code)(arena,0,arena+128);REQUIRE(cursor==events&&!memcmp(arena,expected,1024));++snapshots;}
        ++cases;
    }
    printf("NATIVE_ENGINE_LAYER_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
