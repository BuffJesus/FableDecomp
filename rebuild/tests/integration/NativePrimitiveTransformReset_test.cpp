#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_transform_reset_aliases.h"
#include "primitive_transform_reset_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Reset(void*,void*);
extern "C" void *ShaderGlobal,*RenderGlobal;
static unsigned char *arena,expected[40960],initial[40960];
static unsigned stage,calls,cases,snapshots,mode,result;static void *expectedShader,*expectedRender;
static void* deviceTable[45];static unsigned identity[16];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Off(void*p){return (unsigned char*)p-arena;}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u stage %u\n",__LINE__,cases,stage);exit(70);}}while(0)
static void Record(){REQUIRE(!memcmp(arena,expected,40960)&&ShaderGlobal==expectedShader&&RenderGlobal==expectedRender);++snapshots;}
extern "C" long __stdcall SetTransform(void*device,unsigned state,const void*matrix){
    REQUIRE(calls++==stage&&device==(void*)W(expected,Off(expectedRender)+0x3cf0)&&state==256&&matrix&&!memcmp(matrix,identity,64));Record();
    if(mode==1){ShaderGlobal=expectedShader=expectedShader==arena+0x8000?arena+0x8400:arena+0x8000;RenderGlobal=expectedRender=expectedRender==arena?arena+0x4000:arena;}
    if(mode==2){unsigned base=Off(expectedRender);unsigned value=W(arena,base+0x3cf0)==Ptr(0x9000)?Ptr(0x9010):Ptr(0x9000);W(arena,base+0x3cf0)=W(expected,base+0x3cf0)=value;}
    if(mode==3){unsigned base=Off(expectedShader);W(arena,base+0x3d8)=W(expected,base+0x3d8)=0x12345678;arena[base+0x1e8]=expected[base+0x1e8]=255;}
    Record();return (long)result;
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<4;++i){unsigned value;switch(retailFixups[i][1]){case 0x1436e14:value=(unsigned)&ShaderGlobal;break;case 0x1436e18:value=(unsigned)&RenderGlobal;break;case 0x9881f0:value=(unsigned)(code+144);break;case 0xa0b140:value=(unsigned)(code+304);break;default:return 8;}*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?value-(unsigned)(code+retailFixups[i][0]+4):value;}
    deviceTable[44]=(void*)&SetTransform;identity[0]=identity[5]=identity[10]=identity[15]=0x3f800000;
    const unsigned results[]={0,0x80004005,0xffffffff},flags[]={0,0xc990,0x80000000,0xffffffff};
    for(unsigned placement=0;placement<2;++placement)for(unsigned f=0;f<4;++f)for(unsigned fill=0;fill<4;++fill)
    for(mode=0;mode<4;++mode)for(unsigned r=0;r<3;++r){
        for(i=0;i<40960;++i)arena[i]=(unsigned char)(i*17+fill*59);
        W(arena,0x3cf0)=Ptr(0x9000);W(arena,0x7cf0)=Ptr(0x9010);W(arena,0x9000)=W(arena,0x9010)=(unsigned)deviceTable;
        W(arena,0x83d8)=W(arena,0x87d8)=flags[f];memcpy(initial,arena,40960);result=results[r];
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,40960);memcpy(expected,arena,40960);ShaderGlobal=expectedShader=arena+0x8000+placement*0x400;RenderGlobal=expectedRender=arena+placement*0x4000;calls=0;
            for(stage=0;stage<3;++stage){unsigned base=Off(expectedShader);memcpy(expected+base+0x1f0,identity,64);W(expected,base+0x3d8)|=0xc990;expected[base+0x1e8]=0;typedef void(__fastcall*Fn)(void*,void*);(run?Reset:(Fn)code)(arena+0x9400,0);Record();}REQUIRE(calls==3);
        }++cases;
    }
    printf("NATIVE_PRIMITIVE_TRANSFORM_RESET_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
