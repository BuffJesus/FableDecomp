#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_render_state_manager.h"
#include "vertex_format_retail.h"
#include "vertex_format_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall SetFormat(void*,void*,unsigned long);
static unsigned char* arena;static unsigned char initial[32768],expected[32768];
static unsigned offset,format,mutation,calls;
static void require(bool v){if(!v)abort();}
static unsigned& word(unsigned at){return *(unsigned*)(arena+at);}
static long __stdcall formatCall(void* self,unsigned value){
    require(!calls++&&self==arena+0x5800&&value==format&&!memcmp(initial,arena,32768));
    if(mutation&1)word(offset+0x3cf0)=(unsigned)(arena+0x5840);
    if(mutation&2)word(offset+0x3cf4)=0x12345678;
    return mutation?0x80004005:0;
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,32768,0x3000,4);require(arena==(void*)0x24000000);
    unsigned char* code=(unsigned char*)VirtualAlloc(0,32,0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
    typedef void (__fastcall* Method)(void*,void*,unsigned long);unsigned cases=0;
    for(unsigned pattern=0;pattern<128;++pattern)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place){
        offset=place?0x1000:0;format=pattern==0?0:pattern==1?0x44:pattern==2?0xffffffff:pattern*0x13579bdf;
        for(unsigned run=0;run<2;++run){
            memset(arena,pattern&1?0x95:0,32768);word(offset+0x3cf0)=(unsigned)(arena+0x5800);
            word(0x5800)=word(0x5840)=(unsigned)(arena+0x6000);word(0x6000+89*4)=(unsigned)formatCall;
            memcpy(initial,arena,32768);memcpy(expected,arena,32768);
            if(mutation&1)*(unsigned*)(expected+offset+0x3cf0)=(unsigned)(arena+0x5840);
            if(mutation&2)*(unsigned*)(expected+offset+0x3cf4)=0x12345678;
            calls=0;Method method=run?SetFormat:(Method)code;method(arena+offset,0,format);
            require(calls==1&&!memcmp(expected,arena,32768));
        }
        ++cases;
    }
    VirtualFree(code,0,0x8000);VirtualFree(arena,0,0x8000);printf("NATIVE_VERTEX_FORMAT_PASS cases=%u snapshots=%u\n",cases,cases*4);return 0;
}
