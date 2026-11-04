#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_post_layer_aliases.h"
#include "primitive_post_layer_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall PostLayer(void*,void*);
extern "C" void *ShaderGlobal,*RenderGlobal,*LightingGlobal;
static unsigned char *arena,expected[49152],initial[49152];
static void* deviceTable[108];static unsigned identity[16];
static unsigned cases,snapshots,mode,renderer,shader,render,lighting,version,pendingPixel,transforms,pixels;
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u mode %u\n",__LINE__,cases,mode);exit(70);}}while(0)
static void Record(){REQUIRE(!memcmp(arena,expected,sizeof(expected)));REQUIRE(ShaderGlobal==arena+shader&&RenderGlobal==arena+render&&LightingGlobal==arena+lighting);++snapshots;}
static void ModelVertex(){
    if(expected[renderer+0x261]!=expected[shader+0x390]){
        unsigned rm=W(expected,shader+0x1d0)-(unsigned)arena;
        expected[shader+0x390]=expected[renderer+0x261]?1:0;W(expected,shader+0x1d8)=0;
        if(expected[renderer+0x261]){expected[shader+0x392]=0;expected[rm+0x3cfd]=0;W(expected,shader+0x3d8)=0xffffffff;}
        else expected[rm+0x3cfd]=1;
    }
}
static void ModelShaders(){
    pendingPixel=0xffffffff;
    if(expected[renderer+0x260]!=expected[shader+0x391]){
        if(expected[renderer+0x260]){if(version>=0x100)expected[shader+0x391]=1;}
        else if(expected[shader+0x391]){pendingPixel=shader;return;}
    }
    ModelVertex();
}
extern "C" long __stdcall SetTransform(void*device,unsigned state,const void*matrix){
    REQUIRE(transforms++==0&&device==(void*)Ptr(0x9000)&&state==256&&matrix&&!memcmp(matrix,identity,64));Record();
    if(mode==1){shader=shader==0x8000?0x8400:0x8000;ShaderGlobal=arena+shader;}
    if(mode==2){arena[renderer+0x260]=expected[renderer+0x260]=0;arena[renderer+0x261]=expected[renderer+0x261]=1;}
    ModelShaders();return mode&1?0x80004005:0;
}
extern "C" long __stdcall SetPixelShader(void*device,void*value){
    REQUIRE(pendingPixel!=0xffffffff&&pixels++==0&&device==(void*)Ptr(0x9000)&&!value);Record();
    if(mode==3){shader=shader==0x8000?0x8400:0x8000;ShaderGlobal=arena+shader;}
    expected[pendingPixel+0x391]=0;W(expected,pendingPixel+0x3d8)&=~3u;W(expected,pendingPixel+0x1e4)=0;
    pendingPixel=0xffffffff;ModelVertex();return 0x80004005;
}
static void ModelStart(){
    W(expected,renderer+0x78)=0xffffffff;
    unsigned state=render+0x2a48,desired=W(expected,state+4),mask=W(expected,render+0x2814);
    if(desired){
        if(!(mask&W(expected,state+8))){unsigned slot=render+8+W(expected,render+0x2808)*8;W(expected,state+8)|=mask;W(expected,slot)=Ptr(state);W(expected,slot+4)=desired;++W(expected,render+0x2808);}
        W(expected,state+4)=0;
        if(!expected[state+16]){expected[state+16]=1;unsigned slot=render+0x2008+W(expected,render+0x280c)*4;W(expected,slot)=Ptr(state);++W(expected,render+0x280c);}
    }
    for(long i=1;i<=(long)W(expected,lighting+0xa0);++i){if((unsigned)i>=W(expected,shader+0x16c))continue;unsigned l=W(expected,shader+0x164)-(unsigned)arena+i*0x38;if(expected[l+0x34]){expected[l+0x34]=0;expected[l+0x35]=1;W(expected,shader+0x3d8)|=0x700;}}
    W(expected,lighting+0xa0)=0;memcpy(expected+shader+0x1f0,identity,64);expected[shader+0x1e8]=0;W(expected,shader+0x3d8)|=0xc990;
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,8192,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned value=0,a=retailFixups[i][1];if(a==0x1436e14)value=(unsigned)&ShaderGlobal;else if(a==0x1436e18)value=(unsigned)&RenderGlobal;else if(a==0x1436e9c)value=(unsigned)&LightingGlobal;else for(unsigned j=0;j<sizeof(retailFunctions)/sizeof(retailFunctions[0]);++j)if(retailFunctions[j][0]==a)value=(unsigned)(code+retailFunctions[j][1]);REQUIRE(value);*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?value-(unsigned)(code+retailFixups[i][0]+4):value;}
    deviceTable[44]=(void*)&SetTransform;deviceTable[107]=(void*)&SetPixelShader;identity[0]=identity[5]=identity[10]=identity[15]=0x3f800000;
    const unsigned b[]={0,1,255};const long counts[]={-1,0,7};
    for(unsigned place=0;place<2;++place)for(unsigned norm=0;norm<4;++norm)for(unsigned bits=0;bits<81;++bits)for(unsigned cap=0;cap<2;++cap)for(mode=0;mode<4;++mode)for(unsigned count=0;count<3;++count){
        render=place*0x4000;renderer=0x9400;lighting=0x9800;version=cap?0x100:0;
        for(i=0;i<sizeof(initial);++i)initial[i]=(unsigned char)(i*17+cases*13);
        W(initial,render+0x2808)=3;W(initial,render+0x280c)=2;W(initial,render+0x2814)=4;W(initial,render+0x2a4c)=norm?0xffffffff:0;W(initial,render+0x2a50)=norm==2?4:1;initial[render+0x2a58]=norm==3?255:0;
        W(initial,render+0x3cf0)=Ptr(0x9000);W(initial,0x3a3c)=W(initial,0x7a3c)=Ptr(0xa000);W(initial,0xa128)=version;W(initial,0x9000)=(unsigned)deviceTable;W(initial,lighting+0xa0)=counts[count];
        unsigned x=bits;initial[renderer+0x260]=b[x%3];x/=3;initial[renderer+0x261]=b[x%3];x/=3;
        for(unsigned s=0x8000;s<=0x8400;s+=0x400){initial[s+0x391]=b[x%3];initial[s+0x390]=b[x/3];W(initial,s+0x1d0)=Ptr(s==0x8000?0:0x4000);W(initial,s+0x1d4)=Ptr(0x9000);W(initial,s+0x1e4)=0;W(initial,s+0x164)=Ptr(0xa400+(s-0x8000));W(initial,s+0x16c)=5;for(i=0;i<8;++i)initial[0xa400+s-0x8000+i*0x38+0x34]=i%2?255:0;}
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));memcpy(expected,initial,sizeof(initial));shader=0x8000+place*0x400;ShaderGlobal=arena+shader;RenderGlobal=arena+render;LightingGlobal=arena+lighting;transforms=pixels=0;pendingPixel=0xffffffff;ModelStart();typedef void(__fastcall*Fn)(void*,void*);(run?PostLayer:(Fn)code)(arena+renderer,0);REQUIRE(transforms==1&&pendingPixel==0xffffffff);Record();}++cases;
    }
    printf("NATIVE_PRIMITIVE_POST_LAYER_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
