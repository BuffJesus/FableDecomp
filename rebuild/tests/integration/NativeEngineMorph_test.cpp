#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine_morph_aliases.h"
#include "engine_morph_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u stage %u\n",__LINE__,cases,stage);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned __fastcall Add(void*,void*,void*,void*);
extern "C" void __fastcall Update(void*,void*,void*,unsigned,unsigned,void*);
extern "C" void *ResourceGlobal,*SurfaceGlobal;
static unsigned char *arena,expected[16384];static unsigned p[8],cases,stage,calls;static void *expectedResource,*expectedSurface,*morphs,*used,*texture,*name;static FILE* output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,16384)&&ResourceGlobal==expectedResource&&SurfaceGlobal==expectedSurface);fwrite(arena,1,16384,output);fwrite(&ResourceGlobal,4,1,output);fwrite(&SurfaceGlobal,4,1,output);}
static void Mutate(){
 if(p[5]==1){ResourceGlobal=expectedResource=arena+((unsigned char*)ResourceGlobal==arena?0x1000:0);SurfaceGlobal=expectedSurface=arena+((unsigned char*)SurfaceGlobal==arena+0x3000?0x3400:0x3000);}
 if(p[5]==2){W(arena,0x2000+0x268)=W(expected,0x2000+0x268)=p[3]^0xffffffff;W(arena,0x2400+0x268)=W(expected,0x2400+0x268)=p[3]+17;W(arena,0x100)=W(expected,0x100)=p[7];}
 if(p[5]==3){SurfaceGlobal=expectedSurface=arena+0x3400;W(arena,0x954)=W(expected,0x954)=(unsigned)(arena+0x2400);W(arena,0x1954)=W(expected,0x1954)=(unsigned)(arena+0x2000);}Record();
}
extern "C" void __fastcall Disk(void*self,void*,void*m,unsigned force,unsigned bits,void*u){REQUIRE((stage==0||stage==2)&&calls++==stage&&self==expectedSurface&&m==morphs&&force==p[1]&&bits==p[2]&&u==used);Record();Mutate();}
extern "C" unsigned __fastcall Custom(void*self,void*,void*t,void*n){REQUIRE(stage==1&&calls++==1&&self==expectedSurface&&t==texture&&n==name);Record();Mutate();return p[7];}
static void ModelScene(){unsigned bank=W(expected,(unsigned char*)expectedResource-arena+0x954)-(unsigned)arena,delay=W(expected,bank+0x268);W(expected,bank+0x194)+=delay;W(expected,bank+0x1d0)+=delay;}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<8;++i){unsigned v;switch(retailFixups[i][1]){case 0x1436e98:v=(unsigned)&ResourceGlobal;break;case 0x1436eac:v=(unsigned)&SurfaceGlobal;break;case 0x9fd0c0:v=(unsigned)(code+48);break;case 0x9fc160:v=(unsigned)(code+96);break;case 0xb2c4e0:v=(unsigned)&Custom;break;case 0xb2d9f0:v=(unsigned)&Disk;break;default:return 8;}*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?v-(unsigned)(code+retailFixups[i][0]+4):v;}
 typedef unsigned(__fastcall*AF)(void*,void*,void*,void*);typedef void(__fastcall*UF)(void*,void*,void*,unsigned,unsigned,void*);bool native=!strcmp(argv[3],"retail");AF add=native?(AF)code:&Add;UF update=native?(UF)(code+16):&Update;
 while(fread(p,4,8,in)==8){memset(arena,0x95,16384);W(arena,0x954)=(unsigned)(arena+0x2000);W(arena,0x1954)=(unsigned)(arena+0x2400);for(unsigned bank=0x2000;bank<=0x2400;bank+=0x400){W(arena,bank+0x268)=p[3];W(arena,bank+0x194)=p[4];W(arena,bank+0x1d0)=p[4]+19;}memcpy(expected,arena,16384);ResourceGlobal=expectedResource=arena+p[0]*0x1000;SurfaceGlobal=expectedSurface=arena+0x3000+p[0]*0x400;morphs=arena+0x100;used=p[6]==0?0:p[6]==1?morphs:arena+0x120;texture=p[6]==1?morphs:arena+0x140;name=p[6]==2?texture:arena+0x160;calls=0;
  for(stage=0;stage<3;++stage){if(stage!=1){ModelScene();update(arena+0x3c00,0,morphs,p[1],p[2],used);}else REQUIRE(add(arena+0x3c00,0,texture,name)==p[7]);Record();}REQUIRE(calls==3);++cases;
 }fclose(in);fclose(output);printf("NATIVE_ENGINE_MORPH_PASS cases=%u\n",cases);return 0;
}
