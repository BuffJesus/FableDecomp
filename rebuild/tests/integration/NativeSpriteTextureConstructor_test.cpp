#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_engine_primitive_2d_sprite.h"
#include "sprite_texture_ctor_retail.h"
#include "sprite_texture_ctor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void* __fastcall SpriteTextureCtor(void*,void*,unsigned __int64,const C2DBoxF&,EEngine2DLayer,CTexture&,EEnginePrimitiveBlendMode,EEngineInterpolationSet,float,float,unsigned char,unsigned char,unsigned char,unsigned char,unsigned long,const C2DVector&,const C2DVector&);
static void require(bool b){if(!b)abort();}
static unsigned storage[4096],initial[4096],expected[4096];
static unsigned pattern(unsigned n){const unsigned values[8]={0,0xffffffff,0x80000000,0x7fc01234,0x7f800000,1,0x3f800000,0xff800000};return n<8?values[n]:n*0x13579bdf;}
static float value(unsigned n){float v;memcpy(&v,&n,4);return v;}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned cases=0;
 for(unsigned n=0;n<256;++n)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill)for(unsigned alias=0;alias<8;++alias){
 memset(storage,fill?0xa9:0,sizeof(storage));unsigned char* bytes=(unsigned char*)storage;unsigned out=place?0x1000:0x100;
 // Each reference may alias its own destination member, including unwritten
 // padding. Other combinations use disjoint input storage.
 unsigned box=(alias&1)?out+12:0x2000,uv0=(alias&2)?out+68:0x2100,uv1=(alias&4)?out+76:0x2200;
 for(unsigned i=0;i<4;++i)*(unsigned*)(bytes+box+4*i)=pattern(n+i);for(i=0;i<2;++i){*(unsigned*)(bytes+uv0+4*i)=pattern(n+4+i);*(unsigned*)(bytes+uv1+4*i)=pattern(n+6+i);}
 memcpy(initial,storage,sizeof(storage));memcpy(expected,storage,sizeof(storage));unsigned char* e=(unsigned char*)expected;*(unsigned*)(e+out)=34;*(unsigned*)(e+out+4)=pattern(n+8);e[out+8]=e[out+9]=e[out+10]=0;memcpy(e+out+12,bytes+box,16);
 *(unsigned*)(e+out+28)=pattern(n+9);*(unsigned*)(e+out+32)=0;*(unsigned*)(e+out+36)=0;*(unsigned*)(e+out+40)=pattern(n+12);*(unsigned*)(e+out+44)=pattern(n+13);*(unsigned*)(e+out+48)=pattern(n+14);
 e[out+55]=(unsigned char)n;e[out+54]=(unsigned char)(n*3);e[out+53]=(unsigned char)(n*5);e[out+52]=(unsigned char)(n*7);e[out+56]=0;*(unsigned*)(e+out+60)=pattern(n+15);*(unsigned*)(e+out+64)=(unsigned)(bytes+0x3000);memcpy(e+out+68,bytes+uv0,8);memcpy(e+out+76,bytes+uv1,8);
 for(unsigned run=0;run<2;++run){memcpy(storage,initial,sizeof(storage));void* result=(run?SpriteTextureCtor:(void*(__fastcall*)(void*,void*,unsigned __int64,const C2DBoxF&,EEngine2DLayer,CTexture&,EEnginePrimitiveBlendMode,EEngineInterpolationSet,float,float,unsigned char,unsigned char,unsigned char,unsigned char,unsigned long,const C2DVector&,const C2DVector&))code)(bytes+out,0,((unsigned __int64)pattern(n+17)<<32)|pattern(n+16),*(C2DBoxF*)(bytes+box),(EEngine2DLayer)pattern(n+9),*(CTexture*)(bytes+0x3000),(EEnginePrimitiveBlendMode)pattern(n+14),(EEngineInterpolationSet)pattern(n+8),value(pattern(n+12)),value(pattern(n+13)),(unsigned char)n,(unsigned char)(n*3),(unsigned char)(n*5),(unsigned char)(n*7),pattern(n+15),*(C2DVector*)(bytes+uv0),*(C2DVector*)(bytes+uv1));require(result==bytes+out&&!memcmp(storage,expected,sizeof(storage)));}
 ++cases;
 }VirtualFree(code,0,0x8000);printf("NATIVE_SPRITE_TEXTURE_CONSTRUCTOR_PASS cases=%u snapshots=%u\n",cases,cases*2);return 0;
}
