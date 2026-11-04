#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colour_quad_aliases.h"
#include "colour_quad_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall RenderQuad(void*,void*,unsigned,unsigned,void*,unsigned,unsigned,void*);
extern "C" {void *StateGlobal,*EffectGlobal;unsigned char Shader[1000];}
static unsigned char *arena,expected[65536],expectedShader[1000];static unsigned p[6],cases,defaults,layouts,draws,previous,initial;static void *expectedState,*expectedEffect;static FILE* output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,65536)&&!memcmp(Shader,expectedShader,1000)&&StateGlobal==expectedState&&EffectGlobal==expectedEffect);fwrite(arena,1,65536,output);fwrite(Shader,1,1000,output);fwrite(&StateGlobal,4,1,output);fwrite(&EffectGlobal,4,1,output);}
static void Store(unsigned off,unsigned value){W(arena,off)=W(expected,off)=value;}
static void PushStates(unsigned base){
 for(unsigned n=0;n<p[1];++n){unsigned entry=base+0x2900+n*20,stack=W(arena,base+0x2808);Store(base+8+stack*8,(unsigned)(arena+entry));Store(base+12+stack*8,17+n);Store(base+0x2808,stack+1);Store(entry+4,77+n);Store(entry+8,W(arena,base+0x2814));arena[entry+16]=expected[entry+16]=(unsigned char)(n%2);arena[entry+17]=expected[entry+17]=0;}
}
extern "C" void __fastcall Defaults(void*self,void*){
 REQUIRE(self==arena+0x8000&&++defaults==1&&layouts==0);W(expected,initial+8)=(unsigned)(arena+initial+0x3a24);W(expected,initial+0x2808)=1;W(expected,initial+0x2814)=p[2]<<1;Record();
 if(p[4]==1)W(Shader,8)=W(expectedShader,8)=0x12345678;previous=W(Shader,8);PushStates(initial);Record();
}
extern "C" void __fastcall SetLayout(void*self,void*,unsigned index){
 REQUIRE(self==Shader&&defaults==1&&layouts<2&&index==(layouts?previous:3));++layouts;W(Shader,8)=W(expectedShader,8)=index;
 if(layouts==1&&p[4]==3){EffectGlobal=expectedEffect=arena+0x8400;Store(0x8200,0x55abcdef);}Record();
}
extern "C" void __fastcall DrawQuad(void*self,void*,unsigned x,unsigned y,void*colour,unsigned width,unsigned height,void*texture){
 REQUIRE(self==expectedEffect&&++draws==1&&layouts==1&&x==p[5]&&y==(p[5]^0x80000000)&&colour==arena+0x8200&&width==0x7fc45678&&height==0xbf800000&&texture==(p[0]?arena+0x8600:0));Record();
 W(Shader,8)=W(expectedShader,8)=0x76543210;
 if(p[4]==2){unsigned other=initial^0x4000;StateGlobal=expectedState=arena+other;Store(other+8,(unsigned)(arena+other+0x3a24));Store(other+0x2808,1);Store(other+0x2814,p[2]<<1);PushStates(other);}Record();
}
static void ModelRestore(unsigned base){
 unsigned n=W(expected,base+0x2808);REQUIRE(n==p[1]+1);
 for(unsigned i=p[1];i>0;--i){unsigned entry=base+0x2900+(i-1)*20;W(expected,entry+8)-=W(expected,base+0x2814);W(expected,entry+4)=17+i-1;if(!expected[entry+16]){expected[entry+16]=1;unsigned count=W(expected,base+0x280c)++;W(expected,base+0x2008+4*count)=(unsigned)(arena+entry);}}
 W(expected,base+0x2808)=0;W(expected,base+0x2814)>>=1;
}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<11;++i){unsigned v;switch(retailFixups[i][1]){case 0x13cb508:v=(unsigned)&StateGlobal;break;case 0x13bc470:v=(unsigned)Shader;break;case 0x13bc478:v=(unsigned)(Shader+8);break;case 0x1436e40:v=(unsigned)&EffectGlobal;break;case 0xb24850:v=(unsigned)&Defaults;break;case 0x98b5e0:v=(unsigned)&SetLayout;break;case 0xb5af80:v=(unsigned)&DrawQuad;break;case 0xa05840:v=(unsigned)(code+128);break;default:return 8;}*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?v-(unsigned)(code+retailFixups[i][0]+4):v;}
 typedef void(__fastcall*Fn)(void*,void*,unsigned,unsigned,void*,unsigned,unsigned,void*);Fn fn=!strcmp(argv[3],"retail")?(Fn)code:&RenderQuad;
 while(fread(p,4,6,in)==6){memset(arena,0x95,65536);memset(Shader,0x95,1000);W(Shader,8)=p[3];memcpy(expectedShader,Shader,1000);for(unsigned b=0;b<=0x4000;b+=0x4000){W(arena,b+0x2808)=W(arena,b+0x280c)=0;W(arena,b+0x2814)=p[2];arena[b+0x3a35]=4;}memcpy(expected,arena,65536);initial=p[0]*0x4000;StateGlobal=expectedState=arena+initial;EffectGlobal=expectedEffect=arena+0x8300;defaults=layouts=draws=0;
 fn(arena+0x8000,0,p[5],p[5]^0x80000000,arena+0x8200,0x7fc45678,0xbf800000,p[0]?arena+0x8600:0);REQUIRE(defaults==1&&layouts==2&&draws==1);ModelRestore((unsigned char*)expectedState-arena);Record();++cases;
 }fclose(in);fclose(output);printf("NATIVE_COLOUR_QUAD_PASS cases=%u\n",cases);return 0;
}
