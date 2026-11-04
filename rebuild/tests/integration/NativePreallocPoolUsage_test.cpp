#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pool_usage_retail.h"
#include "pool_usage_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned long __fastcall AllocatedRoot(void*,void*);
extern "C" unsigned long __fastcall InUseRoot(void*,void*,unsigned long);
static unsigned arena[4096],initial[4096];
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 *(unsigned*)(code+callSite)=(unsigned)(code+128)-(unsigned)(code+callSite+4);
 const unsigned values[]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};const unsigned char flags[]={0,1,0x80,0xff};
 unsigned cases=0;
 for(unsigned count=0;count<6;++count)for(unsigned f=0;f<4;++f)for(unsigned frame=0;frame<8;++frame)for(unsigned age=0;age<8;++age)for(unsigned limit=0;limit<8;++limit)for(unsigned links=0;links<4;++links)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
  memset(arena,fill?0xa9:0,sizeof(arena));unsigned base=(unsigned)arena+(place?0x1800:0),pool=base+0x100,array=base+0x300,list=base+0x500;word(pool,0x3c)=count;word(pool,0x48)=count?array:0xffffffff;word(list,0x2c)=values[frame];unsigned allocated=0,used=0;
  for(unsigned i=0;i<count;++i){unsigned node=base+0x700+i*0x80;word(array,i*4)=node;word(node,8)=((links>> (i&1))&1)?list:0;word(node,0x18)=values[(age+i)&7];*(unsigned char*)(node+0x48)=flags[(f+i)&3];if(flags[(f+i)&3]){++allocated;unsigned unused=word(node,8)?values[frame]-values[(age+i)&7]:0xffffffff;if(unused<=values[limit])++used;}}
  memcpy(initial,arena,sizeof(arena));require(((unsigned(__fastcall*)(void*,void*))code)((void*)pool,0)==allocated);require(!memcmp(initial,arena,sizeof(arena)));require(AllocatedRoot((void*)pool,0)==allocated);require(!memcmp(initial,arena,sizeof(arena)));
  require(((unsigned(__fastcall*)(void*,void*,unsigned))(code+64))((void*)pool,0,values[limit])==used);require(!memcmp(initial,arena,sizeof(arena)));require(InUseRoot((void*)pool,0,values[limit])==used);require(!memcmp(initial,arena,sizeof(arena)));++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_PREALLOC_POOL_USAGE_PASS cases=%u snapshots=%u\n",cases,cases*4);return 0;
}
