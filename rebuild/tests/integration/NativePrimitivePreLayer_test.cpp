#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_pre_layer_aliases.h"
#include "primitive_pre_layer_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall PreLayer(void*,void*,unsigned);
extern "C" void *ShaderGlobal,*RenderGlobal,*LightingGlobal;
extern "C" unsigned TextureTable[24];
static unsigned char *arena,expected[49152],initial[49152];
static void* deviceTable[108];static unsigned identity[16];
static unsigned cases,snapshots,mode,renderer,shader,render,lighting,version,layer,pendingPixel,pixels,globals;
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u mode %u layer %x\n",__LINE__,cases,mode,layer);exit(70);}}while(0)
static void Record(){if(memcmp(arena,expected,sizeof(expected))){for(unsigned d=0;d<sizeof(expected);++d)if(arena[d]!=expected[d]){fprintf(stderr,"offset %x actual %x expected %x\n",d,arena[d],expected[d]);break;}}REQUIRE(!memcmp(arena,expected,sizeof(expected)));REQUIRE(ShaderGlobal==arena+shader&&RenderGlobal==arena+render&&LightingGlobal==arena+lighting);for(unsigned i=0;i<24;++i)REQUIRE(TextureTable[i]==i+1);++snapshots;}
static void State(unsigned offset,unsigned value){
    unsigned state=render+offset,desired=W(expected,state+4),mask=W(expected,render+0x2814);
    if(desired!=value){
        if(!(mask&W(expected,state+8))){unsigned slot=render+8+W(expected,render+0x2808)*8;W(expected,state+8)|=mask;W(expected,slot)=Ptr(state);W(expected,slot+4)=desired;++W(expected,render+0x2808);}
        W(expected,state+4)=value;
        if(!expected[state+16]){expected[state+16]=1;unsigned slot=render+0x2008+W(expected,render+0x280c)*4;W(expected,slot)=Ptr(state);++W(expected,render+0x280c);}
    }
}
static void ModelBounding(){
    if(!expected[shader+0x1e8]){memcpy(expected+shader+0x1f0,identity,64);W(expected,shader+0x3d8)|=0xc990;expected[shader+0x1e8]=1;}
    State(0x3204,1);State(0x32a4,1);State(0x2840,0);State(0x2944,0);State(0x2854,0);State(0x28cc,0);State(0x28b8,0);
}
static void ModelStart(){
    expected[renderer+0x261]=expected[shader+0x390];expected[renderer+0x260]=expected[shader+0x391];
    if(layer==2)return;
    if(expected[shader+0x390]!=1){unsigned rm=W(expected,shader+0x1d0)-(unsigned)arena;expected[shader+0x390]=1;W(expected,shader+0x1d8)=0;expected[shader+0x392]=0;expected[rm+0x3cfd]=0;W(expected,shader+0x3d8)=0xffffffff;}
    if(layer==0x1000){if(expected[shader+0x391]){pendingPixel=shader;return;}ModelBounding();}
    else{if(expected[shader+0x391]!=1&&version>=0x100)expected[shader+0x391]=1;if(layer==8||layer==16||layer==32||layer==0x400||layer==0x40000||layer==4||layer==64)State(0x2944,1);}
}
extern "C" long __stdcall SetPixelShader(void*device,void*value){
    REQUIRE(pendingPixel!=0xffffffff&&pixels++==0&&device==(void*)Ptr(0x9000)&&!value);Record();
    if(mode==1){shader=shader==0x8000?0x8400:0x8000;ShaderGlobal=arena+shader;render=render?0:0x4000;RenderGlobal=arena+render;}
    expected[pendingPixel+0x391]=0;W(expected,pendingPixel+0x3d8)&=~3u;W(expected,pendingPixel+0x1e4)=0;
    pendingPixel=0xffffffff;ModelBounding();return 0x80004005;
}
extern "C" void __fastcall GlobalLights(void*self,void*){
    REQUIRE(globals++==0&&self==arena+renderer&&layer!=2);Record();
    if(mode==2){arena[renderer+0xbd]=expected[renderer+0xbd]=arena[renderer+0xbd]?0:255;}
    W(expected,renderer+0x78)=expected[renderer+0xbd]?0:0xffffffff;
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,8192,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned value=0,a=retailFixups[i][1];if(a==0x1436e14)value=(unsigned)&ShaderGlobal;else if(a==0x1436e18)value=(unsigned)&RenderGlobal;else if(a==0x1436e9c)value=(unsigned)&LightingGlobal;else if(a==0x1396e70)value=(unsigned)TextureTable;else if(a==0xb32300)value=(unsigned)&GlobalLights;else for(unsigned j=0;j<sizeof(retailFunctions)/sizeof(retailFunctions[0]);++j)if(retailFunctions[j][0]==a)value=(unsigned)(code+retailFunctions[j][1]);REQUIRE(value);*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?value-(unsigned)(code+retailFixups[i][0]+4):value;}
    deviceTable[107]=(void*)&SetPixelShader;identity[0]=identity[5]=identity[10]=identity[15]=0x3f800000;
    const unsigned b[]={0,1,255},layers[]={0,1,2,4,8,16,32,64,128,0x400,0x1000,0x40000,0x80000000,0xffffffff};
    const unsigned states[]={0x3204,0x32a4,0x2840,0x2944,0x2854,0x28cc,0x28b8};
    for(unsigned place=0;place<2;++place)for(unsigned norm=0;norm<4;++norm)for(unsigned bits=0;bits<9;++bits)for(unsigned cap=0;cap<2;++cap)for(mode=0;mode<3;++mode)for(unsigned l=0;l<14;++l){
        renderer=0x9400;lighting=0x9800;version=cap?0x100:0;layer=layers[l];
        for(i=0;i<sizeof(initial);++i)initial[i]=(unsigned char)(i*17+cases*13);
        for(unsigned rm=0;rm<=0x4000;rm+=0x4000){W(initial,rm+0x2808)=3;W(initial,rm+0x280c)=2;W(initial,rm+0x2814)=4;for(i=0;i<7;++i){unsigned st=rm+states[i];W(initial,st+4)=norm==0?0:norm==1?1:0xffffffff;W(initial,st+8)=norm==2?4:1;initial[st+16]=norm==3?255:0;}W(initial,rm+0x3a3c)=Ptr(0xa000);}
        W(initial,0xa128)=version;W(initial,0x9000)=(unsigned)deviceTable;initial[renderer+0xbd]=b[bits%3];
        for(unsigned s=0x8000;s<=0x8400;s+=0x400){initial[s+0x391]=b[bits%3];initial[s+0x390]=b[bits/3];initial[s+0x1e8]=b[norm%3];W(initial,s+0x1d0)=Ptr(s==0x8000?0:0x4000);W(initial,s+0x1d4)=Ptr(0x9000);W(initial,s+0x1e4)=0;}
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));memcpy(expected,initial,sizeof(initial));shader=0x8000+place*0x400;render=place*0x4000;ShaderGlobal=arena+shader;RenderGlobal=arena+render;LightingGlobal=arena+lighting;pixels=globals=0;pendingPixel=0xffffffff;ModelStart();typedef void(__fastcall*Fn)(void*,void*,unsigned);(run?PreLayer:(Fn)code)(arena+renderer,0,layer);REQUIRE(globals==(layer==2?0:1)&&pendingPixel==0xffffffff);Record();}++cases;
    }
    printf("NATIVE_PRIMITIVE_PRE_LAYER_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
