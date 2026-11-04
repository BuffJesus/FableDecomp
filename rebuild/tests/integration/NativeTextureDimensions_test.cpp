#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_surface_types.h"
#include "texture_dimensions_retail.h"
#include "texture_dimensions_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned __fastcall TextureDimensions(void*,void*,void*,unsigned long);
static unsigned arena[4096],initial[4096],expected[4096],selfPointer,texturePointer,outPointer,level,wantedWidth,wantedHeight,mutation,callbackCalls;
static long status;
static void require(bool b){if(!b)abort();}
static long __stdcall LevelDesc(void* self,unsigned long requested,D3DSURFACE_DESC* out){
 require((unsigned)self==texturePointer&&requested==level);++callbackCalls;memset(out,0xa9,sizeof(*out));out->Width=wantedWidth;out->Height=wantedHeight;
 if(mutation){*(unsigned*)selfPointer=texturePointer+0x100;*(unsigned*)(selfPointer+4)^=0x13579bdf;}
 // Check the entire arena at the callback boundary before the hidden result
 // can overwrite an aliased texture wrapper or interface object.
 require(!memcmp(arena,expected,sizeof(arena)));return status;
}
static unsigned pattern(unsigned n){const unsigned values[8]={0,1,0xffffffff,0x80000000,0x7fffffff,0x10000,0xffff,0x12345678};return n<8?values[n]:n*0x13579bdf;}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned table[18];memset(table,0,sizeof(table));table[17]=(unsigned)LevelDesc;unsigned cases=0;
 const long results[4]={0,1,(long)0x80004005,(long)0x8876086c};
 for(unsigned l=0;l<8;++l)for(unsigned w=0;w<16;++w)for(unsigned h=0;h<16;++h)for(unsigned hr=0;hr<4;++hr)for(mutation=0;mutation<2;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill)for(unsigned alias=0;alias<3;++alias){
 unsigned char* bytes=(unsigned char*)arena;memset(arena,fill?0xa9:0,sizeof(arena));selfPointer=(unsigned)(bytes+(place?0x1000:0x100));texturePointer=(unsigned)(bytes+0x3000);outPointer=alias==0?(unsigned)(bytes+0x2000):alias==1?selfPointer:texturePointer;
 *(unsigned*)selfPointer=texturePointer;*(unsigned*)texturePointer=(unsigned)table;level=pattern(l);wantedWidth=pattern(w);wantedHeight=pattern(h);status=results[hr];memcpy(initial,arena,sizeof(arena));memcpy(expected,initial,sizeof(arena));if(mutation){*(unsigned*)((unsigned)expected+selfPointer-(unsigned)arena)=texturePointer+0x100;*(unsigned*)((unsigned)expected+selfPointer-(unsigned)arena+4)^=0x13579bdf;}
 for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(arena));callbackCalls=0;unsigned result=(run?TextureDimensions:(unsigned(__fastcall*)(void*,void*,void*,unsigned long))code)((void*)selfPointer,0,(void*)outPointer,level);require(result==outPointer&&callbackCalls==1);unsigned before0=*(unsigned*)((unsigned)expected+outPointer-(unsigned)arena),before1=*(unsigned*)((unsigned)expected+outPointer-(unsigned)arena+4);*(unsigned*)((unsigned)expected+outPointer-(unsigned)arena)=wantedWidth;*(unsigned*)((unsigned)expected+outPointer-(unsigned)arena+4)=wantedHeight;require(!memcmp(arena,expected,sizeof(arena)));*(unsigned*)((unsigned)expected+outPointer-(unsigned)arena)=before0;*(unsigned*)((unsigned)expected+outPointer-(unsigned)arena+4)=before1;}
 ++cases;
 }VirtualFree(code,0,0x8000);printf("NATIVE_TEXTURE_DIMENSIONS_PASS cases=%u snapshots=%u\n",cases,cases*4);return 0;
}
