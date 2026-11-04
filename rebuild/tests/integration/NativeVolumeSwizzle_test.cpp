#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_volume_types.h"
#include "volume_swizzle_retail.h"
#include "volume_swizzle_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall VolumeSwizzle(void*,void*,CVolumeLockedBox&);
static unsigned arena[4096],initial[4096];
static void require(bool value){if(!value)abort();}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);
 require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 const unsigned values[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};
 const unsigned boxes[4]={0x600,0x100,0x104,0x400};unsigned cases=0,snapshots=0;
 for(unsigned profile=0;profile<8;++profile)for(unsigned out=0;out<4;++out)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
  unsigned location=place?0x1800:0;unsigned char* base=(unsigned char*)arena+location;
  memset(arena,fill?0xa9:0,sizeof(arena));
  for(unsigned word=0;word<6;++word)((unsigned*)(base+boxes[out]))[word]=values[(profile+word)&7];
  memcpy(initial,arena,sizeof(arena));
  // The independent model preserves every byte; the original has only RET 4.
  for(unsigned mode=0;mode<3;++mode){
   memcpy(arena,initial,sizeof(arena));
   if(mode)(mode==1?((void(__fastcall*)(void*,void*,CVolumeLockedBox&))code):VolumeSwizzle)(base+0x100,0,*(CVolumeLockedBox*)(base+boxes[out]));
   require(!memcmp(initial,arena,sizeof(arena)));if(mode)++snapshots;
  }++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_SWIZZLE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
