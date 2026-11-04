#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_display_engine.h"
#include "display_end_fade_retail.h"
#pragma comment(linker,"/alternatename:@EndFade@8=?EndScreenFadeOut@CDisplayEngine@@QAEXXZ")
extern "C" void __fastcall EndFade(void*,void*);
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static void require(bool b){if(!b)abort();}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,64,0x3000,0x40);require(code!=0);memcpy(code,retailCode,59);*(void**)(code+9)=&GInitialFadeAllowedToBeActive;
 unsigned arena[2048],before[2048],expected[2048];unsigned cases=0;
 for(unsigned flags=0;flags<8;++flags)for(unsigned global=0;global<2;++global)for(unsigned old=0;old<4;++old)for(unsigned bits=0;bits<16;++bits)for(unsigned pos=0;pos<2;++pos)for(unsigned fill=0;fill<2;++fill){
  memset(arena,fill?0x95:0,sizeof(arena));unsigned char* p=(unsigned char*)arena+(pos?0x200:0);p[0xbc]=(flags&1)?1:0;p[0xd8]=(flags&2)?1:0;p[0xec]=(flags&4)?1:0;p[0xc8]=(old&1)?1:0;p[0xc9]=(old&2)?1:0;
  unsigned value=bits==0?0:bits==1?0x7fc01234:bits==2?0x80000000:bits==3?0x7f800000:bits*0x13579bdf;*(unsigned*)(p+0xf0)=value;*(unsigned*)(p+0xf4)=~value;
  memcpy(before,arena,sizeof(arena));memcpy(expected,arena,sizeof(arena));unsigned char* q=(unsigned char*)expected+(p-(unsigned char*)arena);q[0xec]=0;
  if((flags&1)&&!(flags&2)){*(unsigned*)(q+0xf0)=0xbf800000;*(unsigned*)(q+0xf4)=0;q[0xc9]=0;q[0xc8]=1;}
  for(unsigned run=0;run<2;++run){memcpy(arena,before,sizeof(arena));GInitialFadeAllowedToBeActive=global!=0;if(run)EndFade(p,0);else ((void(__fastcall*)(void*,void*))code)(p,0);require(!GInitialFadeAllowedToBeActive);require(!memcmp(arena,expected,sizeof(arena)));}
  ++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_DISPLAY_END_FADE_PASS cases=%u snapshots=%u\n",cases,cases*2);return 0;
}
