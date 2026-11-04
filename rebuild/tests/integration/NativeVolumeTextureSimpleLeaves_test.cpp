#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "volume_texture_simple_leaves_retail.h"
#include "volume_texture_simple_leaves_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall TexturePreLoad(void*,void*);
extern "C" bool __fastcall TextureIsBusy(void*,void*);
extern "C" bool __fastcall TextureCompress(void*,void*);
extern "C" void __fastcall TextureGenerateMipmaps(void*,void*,unsigned long,unsigned long);
static unsigned arena[4096],initial[4096],images[2][4096];
static unsigned mode,step,count,snapshots,location,profile,mutation,calls;
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned address,unsigned offset){return *(unsigned*)(address+offset);}
static void require(bool value){if(!value)abort();}
static unsigned pattern(unsigned n){const unsigned values[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static void snapshot(){if(!mode){require(count<2);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static void __stdcall preload(void* self){
 require(!calls&&(unsigned)self==word(ptr(0x100),4));++calls;word(ptr(0x2000),0)=calls;word(ptr(0x2000),4)=(unsigned)self;
 if(mutation&1){word(ptr(0x100),4)=pattern(profile);word(ptr(0x100),0)^=0xffffffff;word(ptr(0x100),8)=pattern(profile+5);word(ptr(0x100),12)=pattern(profile+6);}
 if(mutation&2){word((unsigned)self,0)=pattern(profile+1);word((unsigned)self,4)=pattern(profile+2);}snapshot();
}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 unsigned table[10]={0};table[9]=(unsigned)preload;unsigned cases=0;
 for(unsigned kind=0;kind<4;++kind)for(profile=0;profile<8;++profile)for(unsigned first=0;first<8;++first)for(unsigned second=0;second<8;++second)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
  location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=kind?pattern(profile+first):ptr(profile&1?0x400:0x800);word(ptr(0x100),8)=pattern(first);word(ptr(0x100),12)=pattern(second);
  word(ptr(0x400),0)=word(ptr(0x800),0)=(unsigned)table;memset((void*)ptr(0x2000),0,16);memcpy(initial,arena,sizeof(arena));count=0;
  for(mode=0;mode<3;++mode){
   memcpy(arena,initial,sizeof(arena));step=calls=0;unsigned result=0;
   if(!mode){if(!kind)preload((void*)word(ptr(0x100),4));}
   else if(!kind)(mode==1?((void(__fastcall*)(void*,void*))(code)):TexturePreLoad)((void*)ptr(0x100),0);
   else if(kind==1)result=(mode==1?((bool(__fastcall*)(void*,void*))(code+32)):TextureIsBusy)((void*)ptr(0x100),0);
   else if(kind==2)result=(mode==1?((bool(__fastcall*)(void*,void*))(code+48)):TextureCompress)((void*)ptr(0x100),0);
   else(mode==1?((void(__fastcall*)(void*,void*,unsigned long,unsigned long))(code+64)):TextureGenerateMipmaps)((void*)ptr(0x100),0,pattern(first),pattern(second));
   require(!result&&calls==(kind?0:1));word(ptr(0x2000),8)=result;snapshot();if(mode)require(step==count);
  }++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_SIMPLE_LEAVES_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
