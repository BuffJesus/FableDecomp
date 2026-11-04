#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEnginePrimitiveRenderer.h"
#include "fable_engine_component_params.h"
#include "primitive_frame_scene_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall EndFrame(void*,void*,const void*);
static unsigned char arena[16384],expected[16384],initial[16384];
struct Event{unsigned kind,self,index;unsigned char state[16384];};static Event events[20];
static unsigned mode,total,cursor,cases,snapshots,base;
static void* primitiveTable[2];static void* managerTable[5];static void* componentTable[13];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
static unsigned Off(unsigned p){return p-(unsigned)arena;}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u\n",__LINE__,cases,cursor);exit(70);}}while(0)
static void Mutate(unsigned char*a,unsigned kind,unsigned index,unsigned self){
    if(mode==1&&kind==1){unsigned n=(W(a,base+0x33c)-W(a,base+0x338))/4;W(a,base+0x338)=Ptr(0x600);W(a,base+0x33c)=Ptr(0x600+4*n);}
    if(mode==2&&kind==0)W(a,base+0x33c)=Ptr(0x500+4*(index+1));
    if(mode==3&&kind==1&&index==0)W(a,base+0x278)=W(a,base+0x274)+40;
    if(mode==4&&kind==1){unsigned n=(W(a,base+0x278)-W(a,base+0x274))/8;W(a,base+0x274)=Ptr(0x800);W(a,base+0x278)=Ptr(0x800+8*n);}
    if(mode==5&&kind==4)W(a,base+0x33c)=W(a,base+0x338)+4*(index+1);
    if(kind==4)memset(a+self,0xdb,48);
    a[base+0x98]=255;
}
static void Callback(unsigned kind,void*self){REQUIRE(cursor<total);Event&e=events[cursor];REQUIRE(kind==e.kind&&self==arena+e.self&&!memcmp(arena,e.state,16384));++snapshots;Mutate(arena,kind,e.index,e.self);++cursor;}
extern "C" void __fastcall MapUpdate(void*self,void*){Callback(0,self);}
extern "C" void __fastcall ManagerEnd(void*self,void*){Callback(1,self);}
extern "C" void __fastcall PrimitiveDelete(void*self,void*){Callback(3,self);}
extern "C" void __cdecl SceneStatsFree(void*self){Callback(4,self);}
static void Add(unsigned kind,unsigned self,unsigned index){REQUIRE(total<20);Event&e=events[total++];e.kind=kind;e.self=self;e.index=index;memcpy(e.state,expected,16384);Mutate(expected,kind,index,self);}
static void ModelScene(unsigned scene,unsigned index){
    unsigned id=(scene-0x1000)/0x100,area=0x1800+id*0x600;
    Add(0,area+0x100,index);
    unsigned node=W(expected,scene+0x24);
    if(node){unsigned p=Off(node);--W(expected,p+4);W(expected,p+0x38)=W(expected,p+0x3c)=W(expected,p+0x40)=W(expected,p+0x44)=0;if(!W(expected,p+4))Add(3,p,index);}
    W(expected,scene+0x24)=0;
}
static void ModelStats(unsigned scene,unsigned index){
    if(W(expected,scene+0x30)){unsigned id=(scene-0x1000)/0x100,area=0x1800+id*0x600;Add(4,area+0x280,index);Add(4,area+0x240,index);W(expected,area+0x204)=0;W(expected,area+0x208)=W(expected,area+0x20c)=Ptr(area+0x200);W(expected,scene+0x30)=0;}
    W(expected,scene+0x38)=W(expected,scene+0x3c)=W(expected,scene+0x40)=0;
}
int main(){
    unsigned char*code=(unsigned char*)VirtualAlloc(0,16384,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned f=0;f<sizeof(retailFixups)/sizeof(retailFixups[0]);++f){unsigned target=0,a=retailFixups[f][1];if(a==0xb947e0)target=(unsigned)&MapUpdate;else if(a==0xbfea14)target=(unsigned)&SceneStatsFree;else for(unsigned j=0;j<sizeof(retailFunctions)/sizeof(retailFunctions[0]);++j)if(retailFunctions[j][0]==a)target=(unsigned)(code+retailFunctions[j][1]);REQUIRE(target);*(unsigned*)(code+retailFixups[f][0])=target-(unsigned)(code+retailFixups[f][0]+4);}
    managerTable[4]=(void*)&ManagerEnd;primitiveTable[1]=(void*)&PrimitiveDelete;
    for(unsigned scenes=0;scenes<4;++scenes)for(unsigned managers=0;managers<4;++managers)for(mode=0;mode<6;++mode)for(unsigned alias=0;alias<3;++alias)for(unsigned slot=0;slot<2;++slot)for(unsigned fill=0;fill<4;++fill){
        base=slot?0x3000:0;for(unsigned i=0;i<16384;++i)arena[i]=(unsigned char)(i*7+fill*41);
        W(arena,base+0x268)=(unsigned)componentTable;W(arena,base+0x338)=Ptr(0x500);W(arena,base+0x33c)=Ptr(0x500+4*scenes);W(arena,base+0x340)=Ptr(0x514);
        W(arena,base+0x274)=Ptr(0x700);W(arena,base+0x278)=Ptr(0x700+8*managers);W(arena,base+0x27c)=Ptr(0x728);
        for(i=0;i<3;++i)W(arena,0xa00+16*i)=(unsigned)managerTable;
        for(i=0;i<5;++i){W(arena,0x500+4*i)=Ptr(0x1000+0x100*(alias?i%3:0));W(arena,0x600+4*i)=Ptr(0x1000+0x100*((i+1)%3));W(arena,0x700+8*i)=Ptr(0xa00+16*(alias?i%3:0));W(arena,0x800+8*i)=Ptr(0xa00+16*((i+1)%3));}
        for(i=0;i<3;++i){unsigned scene=0x1000+i*0x100,area=0x1800+i*0x600;
            W(arena,scene+4)=Ptr(area);W(arena,scene+8)=1;W(arena,area)=0;W(arena,area+4)=W(arena,area+8)=W(arena,area+12)=Ptr(area+0x20);
            W(arena,area+0x20)=1;W(arena,area+0x24)=Ptr(area);W(arena,area+0x28)=W(arena,area+0x2c)=0;W(arena,area+0x34)=Ptr(area+0x40);W(arena,area+0x38)=1;
            W(arena,area+0x40)=0;W(arena,area+0x44)=W(arena,area+0x48)=W(arena,area+0x4c)=Ptr(area+0x60);
            W(arena,area+0x60)=1;W(arena,area+0x64)=Ptr(area+0x40);W(arena,area+0x68)=W(arena,area+0x6c)=0;W(arena,area+0x74)=Ptr(area+0x100);
            W(arena,scene+0x24)=Ptr(area+0x180);W(arena,area+0x180)=(unsigned)primitiveTable;W(arena,area+0x184)=fill;W(arena,area+0x1b8)=0;
            W(arena,scene+0x2c)=Ptr(area+0x200);W(arena,scene+0x30)=2;W(arena,area+0x200)=0;W(arena,area+0x204)=W(arena,area+0x208)=Ptr(area+0x240);W(arena,area+0x20c)=Ptr(area+0x280);
            W(arena,area+0x240)=1;W(arena,area+0x244)=Ptr(area+0x200);W(arena,area+0x248)=0;W(arena,area+0x24c)=Ptr(area+0x280);
            W(arena,area+0x280)=1;W(arena,area+0x284)=Ptr(area+0x240);W(arena,area+0x288)=W(arena,area+0x28c)=0;
        }
        memcpy(initial,arena,16384);memcpy(expected,arena,16384);total=0;
        unsigned index=0;for(unsigned it=Off(W(expected,base+0x338));it!=Off(W(expected,base+0x33c));it+=4)ModelScene(Off(W(expected,it)),index++);
        for(i=0;i<(W(expected,base+0x278)-W(expected,base+0x274))/8;++i)Add(1,Off(W(expected,Off(W(expected,base+0x274))+8*i)),i);
        for(i=0;i<(W(expected,base+0x33c)-W(expected,base+0x338))/4;++i)ModelStats(Off(W(expected,Off(W(expected,base+0x338))+4*i)),i);
        expected[base+0x98]=0;
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,16384);cursor=0;componentTable[2]=run?(void*)&EndFrame:code;CEngineComponent* component=static_cast<CEngineComponent*>(reinterpret_cast<CEnginePrimitiveRenderer*>(arena+base));REQUIRE((void*)component==arena+base+0x268);component->FrameEnd(*reinterpret_cast<const CEngineComponentParams*>(arena+0x2a00));REQUIRE(cursor==total&&!memcmp(arena,expected,16384));++snapshots;}++cases;
    }
    printf("NATIVE_PRIMITIVE_FRAME_SCENE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
