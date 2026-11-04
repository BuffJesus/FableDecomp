#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wait_gpu_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall WaitRoot(void*,void*);
static unsigned arena[4096],initial[4096];
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,1,0x3000,0x40);if(!code)abort();code[0]=0xc3;unsigned cases=0;
 for(unsigned raw=0;raw<6;++raw)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){memset(arena,fill?0xa9:0,sizeof(arena));memcpy(initial,arena,sizeof(arena));unsigned values[]={0,1,0x80000000,0xffffffff,(unsigned)arena,(unsigned)arena+(place?0x1800:0)+0x100};void* self=(void*)values[raw];((void(__fastcall*)(void*,void*))code)(self,0);if(memcmp(arena,initial,sizeof(arena)))abort();WaitRoot(self,0);if(memcmp(arena,initial,sizeof(arena)))abort();++cases;}
 VirtualFree(code,0,0x8000);printf("NATIVE_RENDER_MANAGER_WAIT_GPU_PASS cases=%u snapshots=%u\n",cases,cases*2);return 0;}
