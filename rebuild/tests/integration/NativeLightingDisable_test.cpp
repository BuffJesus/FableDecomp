#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lighting_disable_aliases.h"
#include "lighting_disable_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall ResetLight(void*,void*,unsigned long);
extern "C" void __fastcall DisablePoints(void*,void*);
extern "C" void* ShaderGlobal;
static unsigned char *arena,expected[16384],initial[16384];
static unsigned cases,snapshots;
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
static void Model(unsigned shader,unsigned index){
    if(index>=W(expected,shader+0x16c))return;
    unsigned light=W(expected,shader+0x164)-(unsigned)arena+index*0x38;
    if(!expected[light+0x34])return;
    expected[light+0x34]=0;expected[light+0x35]=1;W(expected,shader+0x3d8)|=0x700;
}
static void Record(unsigned shader){REQUIRE(!memcmp(arena,expected,sizeof(expected))&&ShaderGlobal==arena+shader);++snapshots;}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned value=retailFixups[i][1]==0x1436e14?(unsigned)&ShaderGlobal:(unsigned)code;REQUIRE(retailFixups[i][1]==0x1436e14||retailFixups[i][1]==0x988640);*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?value-(unsigned)(code+retailFixups[i][0]+4):value;}
    const long counts[]={-2147483647-1,-1,0,1,4,8};
    const unsigned sizes[]={0,1,2,5,8,12},flags[]={0,0x700,0x80000000,0xffffffff},indices[]={0,1,7,11,12,0xffffffff};
    for(unsigned place=0;place<2;++place)for(unsigned c=0;c<6;++c)for(unsigned size=0;size<6;++size)for(unsigned f=0;f<4;++f)for(unsigned fill=0;fill<4;++fill){
        unsigned shader=place*0x400,manager=0x1000+place*0x100,lights=0x2000+place*0x400;
        for(i=0;i<sizeof(initial);++i)initial[i]=(unsigned char)(i*17+fill*59);
        W(initial,shader+0x164)=(unsigned)(arena+lights);W(initial,shader+0x16c)=sizes[size];W(initial,shader+0x3d8)=flags[f];W(initial,manager+0xa0)=counts[c];
        for(i=0;i<12;++i){initial[lights+i*0x38+0x34]=(unsigned char)((i+fill)%4==0?0:(i+fill)%4==1?1:(i+fill)%4==2?128:255);}
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));memcpy(expected,initial,sizeof(initial));ShaderGlobal=arena+shader;
            typedef void(__fastcall*ResetFn)(void*,void*,unsigned long);typedef void(__fastcall*DisableFn)(void*,void*);
            for(i=0;i<6;++i){Model(shader,indices[i]);(run?ResetLight:(ResetFn)code)(arena+shader,0,indices[i]);Record(shader);}
            for(unsigned repeat=0;repeat<2;++repeat){for(long n=0;n<(long)W(expected,manager+0xa0);++n)Model(shader,n+1);W(expected,manager+0xa0)=0;(run?DisablePoints:(DisableFn)(code+64))(arena+manager,0);Record(shader);}
        }++cases;
    }
    printf("NATIVE_LIGHTING_DISABLE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
