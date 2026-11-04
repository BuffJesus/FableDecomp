#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "engine_frame_aliases.h"
#include "engine_frame_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Frame(void*,void*,const void*);
extern "C" { void* StateGlobal; }
static unsigned char *arena,expected[65536],initial[65536],params[128];
static unsigned p[8],cases,snapshots,cursor,events;static unsigned long long timeBits;
static void* modelState;
struct Event {unsigned kind,self,value;void* state;unsigned char memory[65536];};
static Event logEntries[40];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Off(unsigned v){return v-(unsigned)arena;}
static unsigned Ptr(unsigned off){return (unsigned)(arena+off);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u\n",__LINE__,cases,cursor);exit(70);}}while(0)
static void Mutate(unsigned kind,unsigned self,unsigned char*a,void*&state){
    if(kind==0&&p[3]==4)W(a,0x8208)=0x40000000;
    if(kind==1&&p[3]==3)W(a,0x80b8)^=1;
    if(kind==1&&p[3]==5)W(a,0x816c)=Ptr(0x8504);
    if(kind==2&&p[3]==2)W(a,0x80b8)=0;
    if(kind==3&&p[3]==1){state=arena+0x4000;W(a,0x4008)=Ptr(0x7a24);W(a,0x6808)=1;W(a,0x6814)=p[7]<<1;}
}
static unsigned ModelEvent(unsigned kind,unsigned self){
    REQUIRE(events<40);Event&e=logEntries[events++];e.kind=kind;e.self=self;e.state=modelState;e.value=kind==1?W(expected,self+4):0;memcpy(e.memory,expected,65536);
    Mutate(kind,self,expected,modelState);return e.value;
}
static unsigned Observe(unsigned kind,void*self,const void*arg){
    REQUIRE(cursor<events);Event&e=logEntries[cursor++];REQUIRE(e.kind==kind&&self==arena+e.self&&StateGlobal==e.state&&!memcmp(arena,e.memory,65536));
    if(kind>=2&&kind<=4)REQUIRE(arg&&!memcmp(arg,params,125));
    ++snapshots;Mutate(kind,e.self,arena,StateGlobal);return e.value;
}
extern "C" void __fastcall Defaults(void*self,void*){Observe(0,self,0);}
extern "C" unsigned __fastcall Peek(void*self,void*){return Observe(1,self,0);}
extern "C" void __fastcall Start(void*self,void*,const void*arg){Observe(2,self,arg);}
extern "C" void __fastcall Layer(void*self,void*,const void*arg){Observe(3,self,arg);}
extern "C" void __fastcall End(void*self,void*,const void*arg){Observe(4,self,arg);}
static void* componentTable[11];static void* layerTable[2];
static void Model(){
    events=0;modelState=arena;W(expected,8)=Ptr(0x3a24);W(expected,0x2808)=1;W(expected,0x2814)=p[7]<<1;
    memcpy(expected+0x80a8,&timeBits,8);ModelEvent(0,0x8000);
    memcpy(params,expected+0x8208,4);memcpy(params+4,expected+0x8200,112);memcpy(params+116,expected+0x8088,4);memcpy(params+120,expected+0x80b8,4);params[124]=expected[0x80f8];
    for(unsigned pos=Off(W(expected,0x8168));pos!=Off(W(expected,0x816c));pos+=4){unsigned self=Off(W(expected,pos));unsigned mask=ModelEvent(1,self);if(mask&W(expected,0x80b8))ModelEvent(2,Off(W(expected,pos)));}
    for(pos=Off(W(expected,0x815c));pos!=Off(W(expected,0x8160));pos+=4)ModelEvent(3,Off(W(expected,pos)));
    for(pos=Off(W(expected,0x8168));pos!=Off(W(expected,0x816c));pos+=4){unsigned self=Off(W(expected,pos));unsigned mask=ModelEvent(1,self);if(mask&W(expected,0x80b8))ModelEvent(4,Off(W(expected,pos)));}
    W(expected,0x8144)=0;
    if(p[5]){W(expected,0x9000)=112;W(expected,0x9004)=0;W(expected,0x9008)=97;W(expected,0x8144)=Ptr(0x9000);}
    ++W(expected,0x8088);unsigned state=(unsigned char*)modelState-arena;W(expected,state+0x2808)=0;W(expected,state+0x2814)>>=1;
}
int main(int argc,char**argv){
    if(argc!=2)return 2;FILE*in=fopen(argv[1],"rb");if(!in)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    static const unsigned unsignedBias=0x4f800000;
    for(unsigned k=0;k<7;++k){unsigned value;switch(retailFixups[k][1]){case 0x13cb508:value=(unsigned)&StateGlobal;break;case 0x122dcb4:value=(unsigned)&unsignedBias;break;case 0xb24850:value=(unsigned)&Defaults;break;case 0x42fa30:value=(unsigned)(code+368);break;case 0xa5c720:value=(unsigned)(code+528);break;case 0xa05840:value=(unsigned)(code+640);break;default:return 8;}*(unsigned*)(code+retailFixups[k][0])=retailFixups[k][2]?value-(unsigned)(code+retailFixups[k][0]+4):value;}
    componentTable[1]=(void*)&Start;componentTable[2]=(void*)&End;componentTable[10]=(void*)&Peek;layerTable[1]=(void*)&Layer;
    while(fread(p,4,8,in)==8){REQUIRE(fread(&timeBits,8,1,in)==1);
        memset(arena,0x95,65536);
        for(unsigned s=0;s<=0x4000;s+=0x4000){W(arena,s+0x2808)=W(arena,s+0x280c)=0;W(arena,s+0x2814)=p[7];arena[s+0x3a35]=4;}
        W(arena,0x8088)=p[4];W(arena,0x80b0)=p[6];W(arena,0x80b8)=p[2];arena[0x80f8]=(unsigned char)(p[3]%2?255:0);
        double base=1.5;memcpy(arena+0x8098,&base,8);W(arena,0x8208)=0x3f800000;
        W(arena,0x8168)=Ptr(0x8500);W(arena,0x816c)=Ptr(0x8500+p[0]*4);W(arena,0x8170)=Ptr(0x8510);
        W(arena,0x815c)=Ptr(0x8520);W(arena,0x8160)=Ptr(0x8520+p[1]*4);W(arena,0x8164)=Ptr(0x8530);
        for(k=0;k<3;++k){W(arena,0x8400+k*16)=(unsigned)componentTable;W(arena,0x8404+k*16)=1<<k;W(arena,0x8500+k*4)=Ptr(0x8400+k*16);W(arena,0x8600+k*16)=(unsigned)layerTable;W(arena,0x8520+k*4)=Ptr(0x8600+k*16);}
        W(arena,0x8134)=p[5]?Ptr(0x8800):0;W(arena,0x8808)=Ptr(0x9000);W(arena,0x8810)=128;W(arena,0x8814)=0;
        memcpy(initial,arena,65536);memcpy(expected,arena,65536);Model();
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,65536);StateGlobal=arena;cursor=0;_control87(0x27f,0xffff);typedef void(__fastcall*Fn)(void*,void*,const void*);(run?Frame:(Fn)code)(arena+0x8000,0,arena+0x8200);REQUIRE(cursor==events&&StateGlobal==modelState&&!memcmp(arena,expected,65536));++snapshots;}
        ++cases;
    }
    fclose(in);printf("NATIVE_ENGINE_FRAME_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
