#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subprimitive_walkers_aliases.h"
#include "subprimitive_walkers_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall One(void*,void*,void*,unsigned);
extern "C" void __fastcall Two(void*,void*,void*,unsigned,unsigned);
extern "C" void __fastcall Delete(void*,void*,void*);
static unsigned char *arena,initial[8192],expected[8192];
struct Event{unsigned self,p,pass,kind;unsigned char state[8192];};static Event events[32];
static unsigned cases,snapshots,total,cursor,mode;
static void* managerTable[6];static void* primitiveTable[1];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
static unsigned Off(unsigned p){return p-(unsigned)arena;}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u mode %u\n",__LINE__,cases,cursor,mode);exit(70);}}while(0)
static void Mutate(unsigned char*a,unsigned p,unsigned kind){
    a[8191]=(unsigned char)(p+kind);
    if(mode==1){W(a,0x104)=W(a,0x100);W(a,p+16)=0;}
    if(mode==2){W(a,0x100)=Ptr(0x300);W(a,0x104)=Ptr(0x300);}
    if(mode==3){unsigned next=W(a,p+16);if(next){unsigned n=Off(next);W(a,n+4)^=4;W(a,n+20)=Ptr(0x740);}}
    if(kind==1)memset(a+p,0xdd,24);
}
static void Call(unsigned kind,void*self,void*p,unsigned pass){
    REQUIRE(cursor<total);Event&e=events[cursor];REQUIRE(kind==e.kind&&self==arena+e.self&&p==arena+e.p&&pass==e.pass&&!memcmp(arena,e.state,sizeof(expected)));++snapshots;Mutate(arena,e.p,kind);++cursor;
}
extern "C" void __fastcall Draw(void*s,void*,void*p,unsigned pass){Call(0,s,p,pass);}
extern "C" void* __fastcall Destroy(void*p,void*,unsigned flags){Call(1,p,p,flags);return p;}
static void Emit(unsigned kind,unsigned p,unsigned pass){
    REQUIRE(total<32);Event&e=events[total++];e.kind=kind;e.self=kind?p:Off(W(expected,p+20));e.p=p;e.pass=pass;memcpy(e.state,expected,sizeof(expected));Mutate(expected,p,kind);
}
static void Model(unsigned action,unsigned pre,unsigned pass){
    unsigned count=(W(expected,0x104)-W(expected,0x100))/8;
    for(unsigned i=0;i<count;++i){
        unsigned first=Off(W(expected,Off(W(expected,0x100))+i*8+4)),p=first;
        if(action==1){do{if(W(expected,p+4)&4)Emit(0,p,pre);p=W(expected,p+16)?Off(W(expected,p+16)):0;}while(p);p=first;}
        do{
            unsigned saved=W(expected,p+16);
            Emit(action==2?1:0,p,action==2?1:pass);
            unsigned next=action==2?saved:W(expected,p+16);p=next?Off(next):0;
        }while(p);
    }
    if(action==2)W(expected,0x104)=W(expected,0x100);
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));managerTable[5]=(void*)&Draw;primitiveTable[0]=(void*)&Destroy;
    for(unsigned count=0;count<4;++count)for(unsigned length=1;length<4;++length)for(unsigned flags=0;flags<8;++flags)for(unsigned passcase=0;passcase<4;++passcase)for(mode=0;mode<4;++mode)for(unsigned action=0;action<3;++action){
        for(unsigned i=0;i<8192;++i)arena[i]=(unsigned char)(i*7+flags*41);
        W(arena,0x100)=Ptr(0x200);W(arena,0x104)=Ptr(0x200+count*8);W(arena,0x108)=Ptr(0x240);
        W(arena,0x700)=W(arena,0x740)=(unsigned)managerTable;
        for(unsigned group=0;group<2;++group)for(i=0;i<3;++i){
            unsigned base=0x800+group*0x400+i*96;W(arena,0x204+group*0x100+i*8)=Ptr(base);
            for(unsigned j=0;j<length;++j){unsigned p=base+j*32;W(arena,p)=(unsigned)primitiveTable;W(arena,p+4)=(flags&(1<<j))?0x80000004:0x80000000;W(arena,p+16)=j+1<length?Ptr(p+32):0;W(arena,p+20)=Ptr(0x700);}
        }
        unsigned pre=passcase==3?13:passcase,pass=passcase==3?0:passcase+1;
        memcpy(initial,arena,sizeof(initial));memcpy(expected,arena,sizeof(expected));total=0;Model(action,pre,pass);
        for(unsigned run=0;run<2;++run){
            memcpy(arena,initial,sizeof(initial));cursor=0;
            typedef void(__fastcall*F1)(void*,void*,void*,unsigned);typedef void(__fastcall*F2)(void*,void*,void*,unsigned,unsigned);typedef void(__fastcall*FD)(void*,void*,void*);
            if(action==0)(run?One:(F1)code)(arena,0,arena+0x100,pass);
            if(action==1)(run?Two:(F2)(code+80))(arena,0,arena+0x100,pre,pass);
            if(action==2)(run?Delete:(FD)(code+192))(arena,0,arena+0x100);
            REQUIRE(cursor==total&&!memcmp(arena,expected,sizeof(expected)));++snapshots;
        }++cases;
    }
    printf("NATIVE_SUBPRIMITIVE_WALKERS_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
