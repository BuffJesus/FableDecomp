#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subprimitive_lists_aliases.h"
#include "subprimitive_lists_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned long __fastcall GetCount(void*,void*,unsigned);
extern "C" void __fastcall ClearType(void*,void*,unsigned);
static unsigned char *arena,initial[4096],expected[4096];
static unsigned cases,snapshots,calls,list,mode;
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
static void Mutate(unsigned char*a){
    W(a,list+4)=W(a,list);
    if(mode==1)W(a,list)=W(a,list+4)=Ptr(0xb00);
    if(mode==2){memset(a+0x800,0xed,64);a[list+23]=0;}
    if(mode==3){W(a,0x180)=Ptr(0xc00);W(a,list+8)=Ptr(0xd00);}
}
extern "C" void __fastcall DeleteList(void*self,void*,void*elements){
    REQUIRE(self==arena&&elements==arena+list&&calls==0&&!memcmp(arena,initial,4096));
    ++calls;++snapshots;Mutate(arena);
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+retailFixup)=(unsigned)&DeleteList-(unsigned)(code+retailFixup+4);
    for(unsigned type=0;type<15;++type)for(unsigned count=0;count<17;++count)for(mode=0;mode<4;++mode)for(unsigned fill=0;fill<4;++fill){
        for(unsigned i=0;i<4096;++i)arena[i]=(unsigned char)(i*7+fill*41);
        for(i=0;i<15;++i){unsigned o=24+i*24;W(arena,o)=Ptr(0x800);W(arena,o+4)=Ptr(0x800+(i+1)*8);W(arena,o+8)=Ptr(0xa00);}
        list=24+type*24;W(arena,list+4)=Ptr(0x800+count*8);
        if(!count&&fill==0)W(arena,list)=W(arena,list+4)=W(arena,list+8)=0;
        memcpy(initial,arena,4096);memcpy(expected,arena,4096);Mutate(expected);
        for(unsigned run=0;run<2;++run){
            memcpy(arena,initial,4096);calls=0;
            typedef unsigned long(__fastcall*Count)(void*,void*,unsigned);
            typedef void(__fastcall*Clear)(void*,void*,unsigned);
            REQUIRE((run?GetCount:(Count)code)(arena,0,type)==count&&!memcmp(arena,initial,4096));++snapshots;
            (run?ClearType:(Clear)(code+32))(arena,0,type);
            REQUIRE(calls==1&&!memcmp(arena,expected,4096));++snapshots;
            REQUIRE((run?GetCount:(Count)code)(arena,0,type)==0&&!memcmp(arena,expected,4096));++snapshots;
        }++cases;
    }
    printf("NATIVE_SUBPRIMITIVE_LISTS_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
