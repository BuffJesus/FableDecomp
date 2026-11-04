#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_nonsorted_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall RenderNonsorted(void*,void*);
typedef void(__fastcall*Fn)(void*,void*);
static unsigned char arena[2048],expected[2048],initial[2048];
struct Event{unsigned self;unsigned char state[2048];};static Event events[8];
static unsigned mode,total,cursor,cases,snapshots;
static void* table[12];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
static unsigned Off(unsigned p){return p-(unsigned)arena;}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u\n",__LINE__,cases,cursor);exit(70);}}while(0)
static void Mutate(unsigned char*a,unsigned call,unsigned self){
    if(mode==1){unsigned n=(W(a,0x278)-W(a,0x274))/8;W(a,0x274)=Ptr(1152);W(a,0x278)=Ptr(1152+8*n);}
    if(mode==2)W(a,0x278)=W(a,0x274)+8;
    if(mode==3&&call==0)W(a,0x278)=W(a,0x274)+40;
    if(mode==4&&call==0){unsigned b=Off(W(a,0x274));W(a,b+8)=Ptr(1568);W(a,b+12)=0xdeadbeef;}
    if(mode==5)a[self+8]^=1;
}
extern "C" void __fastcall Draw(void*self,void*){
    REQUIRE(cursor<total);Event&e=events[cursor];REQUIRE(self==arena+e.self&&!memcmp(arena,e.state,2048));++snapshots;Mutate(arena,cursor,e.self);++cursor;
}
int main(){
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));table[11]=(void*)&Draw;
    for(unsigned count=0;count<5;++count)for(mode=0;mode<6;++mode)for(unsigned alias=0;alias<3;++alias)
    for(unsigned slot=0;slot<2;++slot)for(unsigned fill=0;fill<4;++fill){
        for(unsigned i=0;i<2048;++i)arena[i]=(unsigned char)(i*7+fill*41);
        unsigned base=slot?1152:1024;W(arena,0x274)=Ptr(base);W(arena,0x278)=Ptr(base+8*count);W(arena,0x27c)=Ptr(base+40);
        for(i=0;i<3;++i){W(arena,1536+16*i)=(unsigned)table;arena[1544+16*i]=(unsigned char)(fill==3?255:fill&1);}
        for(i=0;i<5;++i){W(arena,1024+8*i)=Ptr(1536+16*(alias?i%3:0));W(arena,1152+8*i)=Ptr(1536+16*((i+1)%3));}
        memcpy(initial,arena,2048);memcpy(expected,arena,2048);total=0;
        for(i=0;i<(W(expected,0x278)-W(expected,0x274))/8;++i){REQUIRE(total<8);Event&e=events[total];e.self=Off(W(expected,Off(W(expected,0x274))+8*i));memcpy(e.state,expected,2048);Mutate(expected,total,e.self);++total;}
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,2048);cursor=0;(run?RenderNonsorted:(Fn)code)(arena,0);REQUIRE(cursor==total&&!memcmp(arena,expected,2048));++snapshots;}
        ++cases;
    }
    printf("NATIVE_PRIMITIVE_NONSORTED_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
