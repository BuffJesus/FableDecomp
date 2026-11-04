#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subprimitive_render_type_aliases.h"
#include "subprimitive_render_type_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall RenderType(void*,void*,unsigned,bool);
extern "C" void *StateGlobal,*RenderGlobal;
extern "C" unsigned AlphaTable[8];
static unsigned char *arena,initial[49152],expected[49152];
struct Event{unsigned kind,self,a,b,c,sg,rg;unsigned char state[49152];};static Event events[10];
enum{SORT,BOUNDING,ONE,TWO,DELETE_LIST,RESTORE};
static unsigned cases,snapshots,total,cursor,mode,list,sg,rg;
static void* managerTable[7];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
static unsigned Off(unsigned p){return p-(unsigned)arena;}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u mode %u\n",__LINE__,cases,cursor,mode);exit(70);}}while(0)
static void Mutate(unsigned char*a,unsigned&state,unsigned&render,unsigned kind){
    a[49151]=(unsigned char)(kind+1);
    if(mode==1){state=state?0:0x4000;render=render?0:0x4000;}
    if(mode==2&&kind==SORT){W(a,list)=Ptr(0x9200);W(a,list+4)=Ptr(0x9218);a[list+20]=255;}
    if(mode==3&&kind==BOUNDING){W(a,list)=Ptr(0x9200);W(a,list+4)=Ptr(0x9200);}
    if(kind==DELETE_LIST)W(a,list+4)=W(a,list);
}
static void Call(unsigned kind,void*self,unsigned a=0,unsigned b=0,unsigned c=0){
    REQUIRE(cursor<total);Event&e=events[cursor];
    REQUIRE(kind==e.kind&&self==arena+e.self&&a==e.a&&b==e.b&&c==e.c);
    REQUIRE(StateGlobal==arena+e.sg&&RenderGlobal==arena+e.rg&&!memcmp(arena,e.state,sizeof(expected)));++snapshots;
    unsigned state=e.sg,render=e.rg;Mutate(arena,state,render,kind);StateGlobal=arena+state;RenderGlobal=arena+render;++cursor;
}
extern "C" void __fastcall Sort(void*s,void*,void*a,bool stable){Call(SORT,s,(unsigned)a,(unsigned char)stable);}
extern "C" void __fastcall Bounding(void*s,void*,void*p){Call(BOUNDING,s,(unsigned)p);}
extern "C" void __fastcall One(void*s,void*,const void*a,unsigned pass){Call(ONE,s,(unsigned)a,pass);}
extern "C" void __fastcall Two(void*s,void*,const void*a,unsigned pre,unsigned pass){Call(TWO,s,(unsigned)a,pre,pass);}
extern "C" void __fastcall Delete(void*s,void*,void*a){Call(DELETE_LIST,s,(unsigned)a);}
extern "C" void __fastcall Restore(void*s,void*){Call(RESTORE,s);}
static void Emit(unsigned kind,unsigned self,unsigned a=0,unsigned b=0,unsigned c=0){
    REQUIRE(total<10);Event&e=events[total++];e.kind=kind;e.self=self;e.a=a;e.b=b;e.c=c;e.sg=sg;e.rg=rg;memcpy(e.state,expected,sizeof(expected));Mutate(expected,sg,rg,kind);
}
static void Model(bool flush){
    unsigned n=W(expected,sg+0x2808);W(expected,sg+8+n*8)=Ptr(sg+0x3a24);W(expected,sg+0x2808)=n+1;W(expected,sg+0x2814)<<=1;
    unsigned info=rg+0x28a4;
    if(W(expected,info+4)!=5){
        unsigned mask=W(expected,rg+0x2814);
        if(!(W(expected,info+8)&mask)){
            W(expected,info+8)|=mask;n=W(expected,rg+0x2808);W(expected,rg+8+n*8)=Ptr(info);W(expected,rg+12+n*8)=W(expected,info+4);W(expected,rg+0x2808)=n+1;
        }
        W(expected,info+4)=5;
        if(!expected[info+16]){expected[info+16]=1;n=W(expected,rg+0x280c);W(expected,rg+0x2008+n*4)=Ptr(info);W(expected,rg+0x280c)=n+1;}
    }
    if(expected[list+23]){
        if(expected[list+22])Emit(SORT,0x8000,Ptr(list),expected[list+21]);
        if(expected[list+20]){
            unsigned count=(W(expected,list+4)-W(expected,list))/8;
            for(unsigned i=0;i<count;++i){unsigned p=Off(W(expected,Off(W(expected,list))+i*8+4));Emit(BOUNDING,Off(W(expected,p+20)),Ptr(p));}
        }else if(W(expected,list+16))Emit(TWO,0x8000,Ptr(list),W(expected,list+16),W(expected,list+12));
        else Emit(ONE,0x8000,Ptr(list),W(expected,list+12));
    }
    if(flush)Emit(DELETE_LIST,0x8000,Ptr(list));
    Emit(RESTORE,sg);
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    unsigned targets[]={(unsigned)&StateGlobal,(unsigned)&RenderGlobal,(unsigned)&AlphaTable[4],(unsigned)&Sort,(unsigned)&Two,(unsigned)&One,(unsigned)&Delete,(unsigned)&Restore};
    for(unsigned i=0;i<9;++i){const unsigned*f=retailFixups[i];*(unsigned*)(code+f[0])=targets[f[1]]-(f[2]?(unsigned)(code+f[0]+4):0);}
    managerTable[6]=(void*)&Bounding;
    for(unsigned type=0;type<15;++type)for(unsigned flags=0;flags<16;++flags)for(unsigned count=0;count<4;++count)for(unsigned statecase=0;statecase<4;++statecase)for(mode=0;mode<4;++mode)for(unsigned flush=0;flush<2;++flush){
        for(i=0;i<49152;++i)arena[i]=(unsigned char)(i*7+type*41);
        for(i=0;i<2;++i){unsigned s=i*0x4000;W(arena,s+0x2808)=2;W(arena,s+0x280c)=3;W(arena,s+0x2814)=4;W(arena,s+0x28a8)=statecase==0?5:7;W(arena,s+0x28ac)=statecase==2?12:0;arena[s+0x28b4]=statecase==3?255:0;}
        list=0x8018+type*24;W(arena,list)=Ptr(0x9000);W(arena,list+4)=Ptr(0x9000+count*8);W(arena,list+8)=Ptr(0x9080);W(arena,list+12)=2;W(arena,list+16)=flags&1?1:0;
        arena[list+20]=flags&2?255:0;arena[list+21]=(unsigned char)(type%3?255:0);arena[list+22]=flags&4?127:0;arena[list+23]=flags&8?255:0;
        for(i=0;i<4;++i){W(arena,0x9004+i*8)=Ptr(0x9400+i*32);W(arena,0x9204+i*8)=Ptr(0x9500+i*32);W(arena,0x9414+i*32)=Ptr(0x9700);W(arena,0x9514+i*32)=Ptr(0x9720);}
        W(arena,0x9700)=W(arena,0x9720)=(unsigned)managerTable;
        memcpy(initial,arena,sizeof(initial));memcpy(expected,arena,sizeof(expected));sg=0;rg=statecase&1?0x4000:0;total=0;Model(flush!=0);
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));StateGlobal=arena;RenderGlobal=arena+(statecase&1?0x4000:0);cursor=0;typedef void(__fastcall*Fn)(void*,void*,unsigned,bool);(run?RenderType:(Fn)code)(arena+0x8000,0,type,flush!=0);REQUIRE(cursor==total&&!memcmp(arena,expected,sizeof(expected))&&StateGlobal==arena+sg&&RenderGlobal==arena+rg);++snapshots;}++cases;
    }
    printf("NATIVE_SUBPRIMITIVE_RENDER_TYPE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
