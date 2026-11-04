#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pool_frame_retail.h"
#include "pool_frame_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall FrameRoot(void*,void*);
extern "C" unsigned long __fastcall CountRoot(void*,void*);
extern "C" void __fastcall UnusedQueue(void*,void*){abort();}
static unsigned arena[4096],initial[4096],expected[4096];
static void require(bool b){if(!b)abort();}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+1)=(unsigned)(code+32)-(unsigned)(code+5);const unsigned values[]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};unsigned cases=0,snapshots=0;
 for(unsigned frame=0;frame<8;++frame)for(unsigned unloaded=0;unloaded<8;++unloaded)for(unsigned count=0;count<8;++count)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){memset(arena,fill?0xa9:0,sizeof(arena));unsigned char* pool=(unsigned char*)arena+(place?0x1800:0)+0x100;*(unsigned*)(pool+0x2c)=values[frame];*(unsigned*)(pool+0x38)=values[unloaded];*(unsigned*)(pool+0x3c)=values[count];memcpy(initial,arena,sizeof(arena));
 require(((unsigned(__fastcall*)(void*,void*))(code+16))(pool,0)==values[count]);require(!memcmp(initial,arena,sizeof(arena)));++snapshots;require(CountRoot(pool,0)==values[count]);require(!memcmp(initial,arena,sizeof(arena)));++snapshots;
 for(unsigned mode=0;mode<2;++mode){memcpy(arena,initial,sizeof(arena));for(unsigned repeat=0;repeat<3;++repeat){if(mode)FrameRoot(pool,0);else ((void(__fastcall*)(void*,void*))code)(pool,0);memcpy(expected,initial,sizeof(arena));unsigned char* model=(unsigned char*)expected+(place?0x1800:0)+0x100;*(unsigned*)(model+0x2c)=values[frame]+repeat+1;*(unsigned*)(model+0x38)=0;require(!memcmp(expected,arena,sizeof(arena)));++snapshots;}}++cases;}
 VirtualFree(code,0,0x8000);printf("NATIVE_PREALLOC_POOL_FRAME_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
