#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEnginePrimitiveRenderer.h"
#include "fable_engine_component_params.h"
#include "primitive_render_aliases.h"
#include "primitive_render_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall RenderPrimitive(void*,void*,const void*);
extern "C" void *StateGlobal,*ShadowGlobal,*OutlineGlobal,*SubGlobal,*DisplaceGlobal,*CameraGlobal;
static unsigned char *arena,expected[49152],initial[49152],clip[152];
struct Event{unsigned kind,self,args[4],result,g[6];unsigned char clip[152],state[49152];};static Event events[32];
enum Kind{PRE,NON_SORTED,PEEK,SHADOW,RENDER_TYPE,COUNT,CLEAR,OUTLINE_BEGIN,OUTLINE_END,DISPLACE_BEGIN,DISPLACE_END,STENCIL_BEGIN,STENCIL_END,SCENE,VISIBLE,POST,RESTORE};
static unsigned mode,total,cursor,cases,snapshots,eg[6],initialg[6],sceneSeen;
static void* componentTable[13];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
static unsigned Off(unsigned p){return p-(unsigned)arena;}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u mode %u\n",__LINE__,cases,cursor,mode);exit(70);}}while(0)
static void GetGlobals(unsigned*g){g[0]=Off((unsigned)StateGlobal);g[1]=Off((unsigned)ShadowGlobal);g[2]=Off((unsigned)OutlineGlobal);g[3]=Off((unsigned)SubGlobal);g[4]=Off((unsigned)DisplaceGlobal);g[5]=Off((unsigned)CameraGlobal);}
static void PutGlobals(const unsigned*g){StateGlobal=arena+g[0];ShadowGlobal=arena+g[1];OutlineGlobal=arena+g[2];SubGlobal=arena+g[3];DisplaceGlobal=arena+g[4];CameraGlobal=arena+g[5];}
static void Mutate(unsigned char*a,unsigned*g,unsigned kind,unsigned type){
    a[49151]=(unsigned char)(kind+1);
    if(mode==1){g[1]=g[1]==0xa000?0xa400:0xa000;g[2]=g[2]==0xa800?0xa900:0xa800;g[3]=g[3]==0xaa00?0xab00:0xaa00;g[4]=g[4]==0xac00?0xad00:0xac00;g[5]=g[5]==0xae00?0xaf00:0xae00;}
    if(mode==2&&kind==PRE){W(a,0x9578)=0;W(a,0x9408)^=3;W(a,0x9404)=W(a,0x9404)==2?32:W(a,0x9404);a[0x90bd]=a[0x90bd]?0:255;a[0x90be]=a[0x90be]?0:255;}
    if(mode==3&&(kind==SHADOW||kind==SCENE||kind==VISIBLE)){++sceneSeen;W(a,0x933c)=Ptr(0x9600+4*sceneSeen);}
    if(mode==4&&kind==RENDER_TYPE&&type==14)W(a,g[3]+0x20+13*4)=0;
}
static unsigned Call(unsigned kind,void*self,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0){
    REQUIRE(cursor<total);Event&e=events[cursor];unsigned g[6];GetGlobals(g);REQUIRE(kind==e.kind&&self==arena+e.self&&a==e.args[0]&&b==e.args[1]&&c==e.args[2]&&d==e.args[3]);REQUIRE(!memcmp(arena,e.state,sizeof(expected))&&!memcmp(g,e.g,sizeof(g)));++snapshots;Mutate(arena,g,kind,a);PutGlobals(g);++cursor;return e.result;
}
extern "C" void __fastcall Pre(void*s,void*,unsigned l){Call(PRE,s,l);}
extern "C" void __fastcall Nonsorted(void*s,void*){Call(NON_SORTED,s);}
extern "C" void* __fastcall Peek(void*s,void*,void*out){REQUIRE(out);memcpy(out,(unsigned char*)s+12,576);Call(PEEK,s);return out;}
extern "C" void __fastcall Shadow(void*s,void*,unsigned l,const void*p){REQUIRE(cursor<total&&p&&!memcmp(p,events[cursor].clip,152));Call(SHADOW,s,l);}
extern "C" void __fastcall RenderType(void*s,void*,unsigned t,bool flush){Call(RENDER_TYPE,s,t,(unsigned char)flush);}
extern "C" unsigned __fastcall Count(void*s,void*,unsigned t){return Call(COUNT,s,t);}
extern "C" void __fastcall Clear(void*s,void*,unsigned t){Call(CLEAR,s,t);}
extern "C" void __fastcall OutlineBegin(void*s,void*){Call(OUTLINE_BEGIN,s);}
extern "C" void __fastcall OutlineEnd(void*s,void*){Call(OUTLINE_END,s);}
extern "C" unsigned __fastcall DisplaceBegin(void*s,void*){return Call(DISPLACE_BEGIN,s);}
extern "C" void __fastcall DisplaceEnd(void*s,void*){Call(DISPLACE_END,s);}
extern "C" void __fastcall StencilBegin(void*s,void*){Call(STENCIL_BEGIN,s);}
extern "C" void __fastcall StencilEnd(void*s,void*){Call(STENCIL_END,s);}
extern "C" void __fastcall Scene(void*s,void*,unsigned l,const void*camera){Call(SCENE,s,l,(unsigned)camera);}
extern "C" void __fastcall Visible(void*s,void*,unsigned l,const void*camera,unsigned occ,bool noHierarchy){Call(VISIBLE,s,l,(unsigned)camera,occ,(unsigned char)noHierarchy);}
extern "C" void __fastcall Post(void*s,void*){Call(POST,s);}
extern "C" void __fastcall Restore(void*s,void*){Call(RESTORE,s);}
static unsigned Emit(unsigned kind,unsigned self,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0){
    REQUIRE(total<32);Event&e=events[total++];e.kind=kind;e.self=self;e.args[0]=a;e.args[1]=b;e.args[2]=c;e.args[3]=d;e.result=kind==COUNT?W(expected,self+0x20+a*4):kind==DISPLACE_BEGIN?expected[self+8]:0;memcpy(e.state,expected,sizeof(expected));memcpy(e.g,eg,sizeof(eg));if(kind==PEEK)memcpy(clip,expected+self+12+272,152);if(kind==SHADOW)memcpy(e.clip,clip,152);Mutate(expected,eg,kind,a);return e.result;
}
static void Draw(unsigned type){Emit(RENDER_TYPE,eg[3],type,1);}
static void Model(){
    if(!(W(expected,0x9578)&1))return;
    unsigned state=eg[0],n=W(expected,state+0x2808);W(expected,state+8+n*8)=Ptr(state+0x3a24);W(expected,state+0x2808)=n+1;W(expected,state+0x2814)<<=1;
    Emit(PRE,0x9000,W(expected,0x9404));unsigned layer=W(expected,0x9404);
    switch(layer){
    case 32:Emit(NON_SORTED,0x9000);break;
    case 2:
        if(expected[eg[1]+8]&&W(expected,eg[1]+0x334))for(unsigned it=Off(W(expected,0x9338));it!=Off(W(expected,0x933c));it+=4){unsigned scene=Off(W(expected,it));Emit(PEEK,eg[1]);Emit(SHADOW,scene,W(expected,0x9404));}
        break;
    case 128:
        if(expected[eg[2]+8]){Draw(14);if(Emit(COUNT,eg[3],13)){Emit(OUTLINE_BEGIN,eg[2]);Draw(13);Emit(OUTLINE_END,eg[2]);}}
        else{Emit(CLEAR,eg[3],14);Emit(CLEAR,eg[3],13);}break;
    case 512:if(expected[eg[2]+8])Draw(9);else Emit(CLEAR,eg[3],9);break;
    case 256:if(expected[eg[2]+8]){Draw(8);Draw(10);}else{Emit(CLEAR,eg[3],8);Emit(CLEAR,eg[3],10);}break;
    case 1024:Draw(11);break;
    case 262144:Draw(12);break;
    case 4096:Draw(7);break;
    case 134217728:
        if(Emit(COUNT,eg[3],5)){if(Emit(DISPLACE_BEGIN,eg[4])){Draw(5);Emit(DISPLACE_END,eg[4]);}Emit(CLEAR,eg[3],5);}break;
    case 268435456:
        if(Emit(COUNT,eg[3],6)){Emit(STENCIL_BEGIN,eg[4]);Draw(6);Emit(STENCIL_END,eg[4]);Emit(CLEAR,eg[3],6);}break;
    default:
        for(unsigned it=Off(W(expected,0x9338));it!=Off(W(expected,0x933c));it+=4){unsigned scene=Off(W(expected,it));if((W(expected,0x9408)&1)&&expected[0x90bd])Emit(VISIBLE,scene,W(expected,0x9404),Ptr(eg[5]),W(expected,0x9078),!expected[0x90be]);else Emit(SCENE,scene,W(expected,0x9404),Ptr(eg[5]));}break;
    }
    Draw(0);if(W(expected,0x9408)&2)Draw(1);Emit(POST,0x9000);Emit(RESTORE,eg[0]);
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    void* addresses[]={&StateGlobal,&ShadowGlobal,&OutlineGlobal,&SubGlobal,&DisplaceGlobal,&CameraGlobal,(void*)&Pre,(void*)&Nonsorted,(void*)&Peek,(void*)&Shadow,(void*)&RenderType,(void*)&Count,(void*)&Clear,(void*)&OutlineBegin,(void*)&OutlineEnd,(void*)&DisplaceBegin,(void*)&DisplaceEnd,(void*)&StencilBegin,(void*)&StencilEnd,(void*)&Scene,(void*)&Visible,(void*)&Post,(void*)&Restore};
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned target=(unsigned)addresses[retailFixups[i][1]];*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?target-(unsigned)(code+retailFixups[i][0]+4):target;}
    const unsigned layers[]={0,2,4,8,32,128,256,512,1024,4096,262144,134217728,268435456,0x80000000,0xffffffff,3},counts[]={0,1,3},booleans[]={0,1,255};
    for(unsigned l=0;l<16;++l)for(unsigned count=0;count<3;++count)for(unsigned flags=0;flags<4;++flags)for(unsigned setting=0;setting<8;++setting)for(mode=0;mode<5;++mode)for(unsigned filter=0;filter<2;++filter){
        for(i=0;i<sizeof(initial);++i)initial[i]=(unsigned char)(i*17+setting*13);W(initial,0x9400)=Ptr(0x9500);W(initial,0x9404)=layers[l];W(initial,0x9408)=flags;W(initial,0x9578)=filter?0xffffffff:2;
        W(initial,0x9268)=(unsigned)componentTable;W(initial,0x9338)=Ptr(0x9600);W(initial,0x933c)=Ptr(0x9600+4*counts[count]);W(initial,0x9340)=Ptr(0x960c);initial[0x90bd]=booleans[setting%3];initial[0x90be]=booleans[(setting/2)%3];W(initial,0x9078)=setting?0xffffffff:0;
        for(i=0;i<3;++i)W(initial,0x9600+4*i)=Ptr(0x9700+(setting&1?i:0)*64);
        W(initial,0x2808)=3;W(initial,0x2814)=setting&1?0x80000000:4;
        for(i=0;i<2;++i){initial[0xa008+i*0x400]=booleans[setting%3];W(initial,0xa334+i*0x400)=setting&2?2:0;initial[0xa808+i*0x100]=booleans[(setting/2)%3];initial[0xac08+i*0x100]=setting&4?255:0;for(unsigned t=0;t<15;++t)W(initial,0xaa20+i*0x100+4*t)=setting&1?0xffffffff:0;}
        initialg[0]=0;initialg[1]=0xa000;initialg[2]=0xa800;initialg[3]=0xaa00;initialg[4]=0xac00;initialg[5]=0xae00;
        memcpy(expected,initial,sizeof(expected));memcpy(eg,initialg,sizeof(eg));total=sceneSeen=0;Model();
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));PutGlobals(initialg);cursor=sceneSeen=0;componentTable[4]=run?(void*)&RenderPrimitive:code;CEngineComponent* component=static_cast<CEngineComponent*>(reinterpret_cast<CEnginePrimitiveRenderer*>(arena+0x9000));REQUIRE((void*)component==arena+0x9268);component->Render(*reinterpret_cast<const CEngineComponentRenderParams*>(arena+0x9400));unsigned g[6];GetGlobals(g);REQUIRE(cursor==total&&!memcmp(arena,expected,sizeof(expected))&&!memcmp(g,eg,sizeof(eg)));++snapshots;}++cases;
    }
    printf("NATIVE_PRIMITIVE_RENDER_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
