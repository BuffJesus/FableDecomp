#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEnginePrimitiveRenderer.h"
#include "fable_engine_component_params.h"
#include "primitive_frame_end_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall EndFrame(void*,void*,const void*);
static unsigned char arena[4096],expected[4096],initial[4096];
struct Event{unsigned kind,self,index;unsigned char state[4096];};static Event events[20];
static unsigned mode,total,cursor,cases,snapshots,base;
static void* managerTable[5];static void* componentTable[13];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
static unsigned Off(unsigned p){return p-(unsigned)arena;}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u\n",__LINE__,cases,cursor);exit(70);}}while(0)
static void Mutate(unsigned char*a,unsigned kind,unsigned index){
    if(mode==1&&kind==1){unsigned n=(W(a,base+0x33c)-W(a,base+0x338))/4;W(a,base+0x338)=Ptr(0x600);W(a,base+0x33c)=Ptr(0x600+4*n);}
    if(mode==2&&kind==0)W(a,base+0x33c)=Ptr(0x500+4*(index+1));
    if(mode==3&&kind==1&&index==0)W(a,base+0x278)=W(a,base+0x274)+40;
    if(mode==4&&kind==1){unsigned n=(W(a,base+0x278)-W(a,base+0x274))/8;W(a,base+0x274)=Ptr(0x800);W(a,base+0x278)=Ptr(0x800+8*n);}
    if(mode==5&&kind==2)W(a,base+0x33c)=W(a,base+0x338)+4*(index+1);
    a[base+0x98]=255;
}
static void Callback(unsigned kind,void*self){REQUIRE(cursor<total);Event&e=events[cursor];REQUIRE(kind==e.kind&&self==arena+e.self&&!memcmp(arena,e.state,4096));++snapshots;Mutate(arena,kind,e.index);++cursor;}
extern "C" void __fastcall SceneEnd(void*self,void*){Callback(0,self);}
extern "C" void __fastcall ManagerEnd(void*self,void*){Callback(1,self);}
extern "C" void __fastcall SceneReset(void*self,void*){Callback(2,self);}
static void Add(unsigned kind,unsigned self,unsigned index){REQUIRE(total<20);Event&e=events[total++];e.kind=kind;e.self=self;e.index=index;memcpy(e.state,expected,4096);Mutate(expected,kind,index);}
int main(){
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+23)=(unsigned)&SceneEnd-(unsigned)(code+27);*(unsigned*)(code+122)=(unsigned)&SceneReset-(unsigned)(code+126);managerTable[4]=(void*)&ManagerEnd;
    for(unsigned scenes=0;scenes<4;++scenes)for(unsigned managers=0;managers<4;++managers)for(mode=0;mode<6;++mode)for(unsigned alias=0;alias<3;++alias)for(unsigned slot=0;slot<2;++slot)for(unsigned fill=0;fill<4;++fill){
        base=slot?0xc00:0;for(unsigned i=0;i<4096;++i)arena[i]=(unsigned char)(i*7+fill*41);
        W(arena,base+0x268)=(unsigned)componentTable;W(arena,base+0x338)=Ptr(0x500);W(arena,base+0x33c)=Ptr(0x500+4*scenes);W(arena,base+0x340)=Ptr(0x514);
        W(arena,base+0x274)=Ptr(0x700);W(arena,base+0x278)=Ptr(0x700+8*managers);W(arena,base+0x27c)=Ptr(0x728);
        for(i=0;i<3;++i)W(arena,0xa00+16*i)=(unsigned)managerTable;
        for(i=0;i<5;++i){W(arena,0x500+4*i)=Ptr(0xb00+16*(alias?i%3:0));W(arena,0x600+4*i)=Ptr(0xb00+16*((i+1)%3));W(arena,0x700+8*i)=Ptr(0xa00+16*(alias?i%3:0));W(arena,0x800+8*i)=Ptr(0xa00+16*((i+1)%3));}
        memcpy(initial,arena,4096);memcpy(expected,arena,4096);total=0;
        unsigned index=0;for(unsigned it=Off(W(expected,base+0x338));it!=Off(W(expected,base+0x33c));it+=4)Add(0,Off(W(expected,it)),index++);
        for(i=0;i<(W(expected,base+0x278)-W(expected,base+0x274))/8;++i)Add(1,Off(W(expected,Off(W(expected,base+0x274))+8*i)),i);
        for(i=0;i<(W(expected,base+0x33c)-W(expected,base+0x338))/4;++i)Add(2,Off(W(expected,Off(W(expected,base+0x338))+4*i)),i);
        expected[base+0x98]=0;
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,4096);cursor=0;componentTable[2]=run?(void*)&EndFrame:code;CEngineComponent* component=static_cast<CEngineComponent*>(reinterpret_cast<CEnginePrimitiveRenderer*>(arena+base));REQUIRE((void*)component==arena+base+0x268);component->FrameEnd(*reinterpret_cast<const CEngineComponentParams*>(arena+0xb80));REQUIRE(cursor==total&&!memcmp(arena,expected,4096));++snapshots;}++cases;
    }
    printf("NATIVE_PRIMITIVE_FRAME_END_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
