#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_algo.h"
#include "fable_stlport_tree.h"
#include "subprimitive_sorted_composed_aliases.h"
#include "subprimitive_sorted_composed_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall RenderType(void*,void*,unsigned,bool);
extern "C" void *StateGlobal,*RenderGlobal;
extern "C" unsigned AlphaTable[8];
static unsigned char *arena,initial[49152],expected[49152];
struct Event{unsigned kind,self,a,b,c,sg,rg;unsigned char state[49152];};static Event events[32];
enum{SORT,BOUNDING,ONE,TWO,DELETE_LIST,RESTORE,DRAW,DESTROY};
static unsigned cases,snapshots,total,cursor,mode,list,sg,rg;
static void* managerTable[7];static void* primitiveTable[1];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
static unsigned Off(unsigned p){return p-(unsigned)arena;}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u mode %u\n",__LINE__,cases,cursor,mode);exit(70);}}while(0)
static void Mutate(unsigned char*a,unsigned&state,unsigned&render,unsigned kind){
    a[49151]=(unsigned char)(kind+1);
    if(mode==1){state=state?0:0x4000;render=render?0:0x4000;}
    if(mode==2&&kind==SORT){W(a,list)=Ptr(0x9200);W(a,list+4)=Ptr(0x9218);a[list+20]=255;}
    if(mode==3&&kind==BOUNDING){W(a,list)=Ptr(0x9200);W(a,list+4)=Ptr(0x9200);}

}
static void Call(unsigned kind,void*self,unsigned a=0,unsigned b=0,unsigned c=0){
    REQUIRE(cursor<total);Event&e=events[cursor];
    REQUIRE(kind==e.kind&&self==arena+e.self&&a==e.a&&b==e.b&&c==e.c);
    REQUIRE(StateGlobal==arena+e.sg&&RenderGlobal==arena+e.rg&&!memcmp(arena,e.state,sizeof(expected)));++snapshots;
    unsigned state=e.sg,render=e.rg;Mutate(arena,state,render,kind);StateGlobal=arena+state;RenderGlobal=arena+render;++cursor;
}
extern "C" void __fastcall Sort(void*,void*,void*,unsigned);
extern "C" void __fastcall Stable(void*first,void*last,void*type,void*distance,unsigned){REQUIRE(!type&&!distance&&first==(void*)W(arena,list)&&last==(void*)W(arena,list+4)&&arena[list+21]);Call(SORT,arena+0x8000,Ptr(list),arena[list+21]);}
extern "C" void __fastcall Ordinary(void*first,void*last){REQUIRE(first==(void*)W(arena,list)&&last==(void*)W(arena,list+4)&&!arena[list+21]);Call(SORT,arena+0x8000,Ptr(list),0);}
extern "C" void __fastcall RadixBoundary(void*,void*,void*){REQUIRE(false);}
namespace _STL {
template<> void FABLE_FASTCALL __stable_sort(CEngineSubPrimitiveRenderer::CSortElement* first,CEngineSubPrimitiveRenderer::CSortElement* last,CEngineSubPrimitiveRenderer::CSortElement* type,ptrdiff_t* distance,less<CEngineSubPrimitiveRenderer::CSortElement>){Stable(first,last,type,distance,0);}
template<> void FABLE_FASTCALL sort(CEngineSubPrimitiveRenderer::CSortElement* first,CEngineSubPrimitiveRenderer::CSortElement* last){Ordinary(first,last);}
}
extern "C" void __fastcall Bounding(void*s,void*,void*p){Call(BOUNDING,s,(unsigned)p);}
extern "C" void __fastcall Draw(void*s,void*,void*p,unsigned pass){Call(DRAW,s,(unsigned)p,pass);}
extern "C" void* __fastcall Destroy(void*p,void*,unsigned flags){Call(DESTROY,p,flags);memset(p,0xdd,24);return p;}
static void Emit(unsigned kind,unsigned self,unsigned a=0,unsigned b=0,unsigned c=0){
    REQUIRE(total<32);Event&e=events[total++];e.kind=kind;e.self=self;e.a=a;e.b=b;e.c=c;e.sg=sg;e.rg=rg;memcpy(e.state,expected,sizeof(expected));Mutate(expected,sg,rg,kind);if(kind==DESTROY)memset(expected+self,0xdd,24);
}
static void DrawList(bool two){
    unsigned count=(W(expected,list+4)-W(expected,list))/8;
    for(unsigned i=0;i<count;++i){
        unsigned first=Off(W(expected,Off(W(expected,list))+i*8+4)),p=first;
        if(two){do{if(W(expected,p+4)&4)Emit(DRAW,Off(W(expected,p+20)),Ptr(p),W(expected,list+16));p=W(expected,p+16)?Off(W(expected,p+16)):0;}while(p);p=first;}
        do{Emit(DRAW,Off(W(expected,p+20)),Ptr(p),W(expected,list+12));p=W(expected,p+16)?Off(W(expected,p+16)):0;}while(p);
    }
}
static void DeleteList(){
    unsigned count=(W(expected,list+4)-W(expected,list))/8;
    for(unsigned i=0;i<count;++i){unsigned p=Off(W(expected,Off(W(expected,list))+i*8+4));do{unsigned next=W(expected,p+16);Emit(DESTROY,p,1);p=next?Off(next):0;}while(p);}
    W(expected,list+4)=W(expected,list);
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
        if(expected[list+22]&&W(expected,list)!=W(expected,list+4))Emit(SORT,0x8000,Ptr(list),expected[list+21]);
        if(expected[list+20]){
            unsigned count=(W(expected,list+4)-W(expected,list))/8;
            for(unsigned i=0;i<count;++i){unsigned p=Off(W(expected,Off(W(expected,list))+i*8+4));Emit(BOUNDING,Off(W(expected,p+20)),Ptr(p));}
        }else DrawList(W(expected,list+16)!=0);
    }
    if(flush)DeleteList();
    unsigned nrestore=--W(expected,sg+0x2808);
    unsigned entry=Off(W(expected,sg+8+nrestore*8));
    while(expected[entry+17]!=4){
        W(expected,entry+8)-=W(expected,sg+0x2814);
        W(expected,entry+4)=W(expected,sg+12+nrestore*8);
        if(!expected[entry+16]){expected[entry+16]=1;unsigned dirty=W(expected,sg+0x280c);W(expected,sg+0x2008+dirty*4)=Ptr(entry);W(expected,sg+0x280c)=dirty+1;}
        nrestore=--W(expected,sg+0x2808);entry=Off(W(expected,sg+8+nrestore*8));
    }
    W(expected,sg+0x2814)>>=1;
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    unsigned targets[]={(unsigned)&StateGlobal,(unsigned)&RenderGlobal,(unsigned)&AlphaTable[4],(unsigned)(code+736),(unsigned)(code+384),(unsigned)(code+304),(unsigned)(code+496),(unsigned)(code+608)};
    for(unsigned i=0;i<9;++i){const unsigned*f=retailFixups[i];*(unsigned*)(code+f[0])=targets[f[1]]-(f[2]?(unsigned)(code+f[0]+4):0);}
    unsigned sortTargets[]={(unsigned)&Stable,(unsigned)&Ordinary,(unsigned)&RadixBoundary};
    for(i=0;i<3;++i){const unsigned*f=sortFixups[i];*(unsigned*)(code+736+f[0])=sortTargets[f[1]]-(unsigned)(code+736+f[0]+4);}
    managerTable[6]=(void*)&Bounding;managerTable[5]=(void*)&Draw;primitiveTable[0]=(void*)&Destroy;
    for(unsigned type=0;type<15;++type)for(unsigned flags=0;flags<16;++flags)for(unsigned count=0;count<4;++count)for(unsigned statecase=0;statecase<4;++statecase)for(mode=0;mode<4;++mode)for(unsigned flush=0;flush<2;++flush){
        for(i=0;i<49152;++i)arena[i]=(unsigned char)(i*7+type*41);
        for(i=0;i<2;++i){unsigned s=i*0x4000;W(arena,s+0x2808)=2;W(arena,s+0x280c)=3;W(arena,s+0x2814)=4;W(arena,s+0x28a8)=statecase==0?5:7;W(arena,s+0x28ac)=statecase==2?12:0;arena[s+0x28b4]=statecase==3?255:0;arena[s+0x28b5]=0;arena[s+0x3a35]=4;W(arena,s+16)=Ptr(s+0x3a24);}
        list=0x8018+type*24;W(arena,list)=Ptr(0x9000);W(arena,list+4)=Ptr(0x9000+count*8);W(arena,list+8)=Ptr(0x9080);W(arena,list+12)=2;W(arena,list+16)=flags&1?1:0;
        arena[list+20]=flags&2?255:0;arena[list+21]=(unsigned char)(type%3?255:0);arena[list+22]=flags&4?127:0;arena[list+23]=flags&8?255:0;
        for(i=0;i<4;++i){W(arena,0x9004+i*8)=Ptr(0x9400+i*32);W(arena,0x9204+i*8)=Ptr(0x9500+i*32);W(arena,0x9414+i*32)=Ptr(0x9700);W(arena,0x9514+i*32)=Ptr(0x9720);}
        for(i=0;i<4;++i){W(arena,0x9400+i*32)=W(arena,0x9500+i*32)=(unsigned)primitiveTable;W(arena,0x9410+i*32)=W(arena,0x9510+i*32)=0;W(arena,0x9404+i*32)=W(arena,0x9504+i*32)=i&1?4:0;}
        W(arena,0x9700)=W(arena,0x9720)=(unsigned)managerTable;
        memcpy(initial,arena,sizeof(initial));memcpy(expected,arena,sizeof(expected));sg=0;rg=statecase&1?0x4000:0;total=0;Model(flush!=0);
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));StateGlobal=arena;RenderGlobal=arena+(statecase&1?0x4000:0);cursor=0;typedef void(__fastcall*Fn)(void*,void*,unsigned,bool);(run?RenderType:(Fn)code)(arena+0x8000,0,type,flush!=0);REQUIRE(cursor==total&&!memcmp(arena,expected,sizeof(expected))&&StateGlobal==arena+sg&&RenderGlobal==arena+rg);++snapshots;}++cases;
    }
    printf("NATIVE_SUBPRIMITIVE_SORTED_COMPOSED_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
