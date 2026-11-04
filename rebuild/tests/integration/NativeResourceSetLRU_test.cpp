#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resource_set_lru_retail.h"
#include "resource_set_lru_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall SetLRU(void*,void*);
static unsigned arena[4096],initial[4096],expected[4096],location;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void model(unsigned self){if(word(self,8)){unsigned next=word(self,16);if(next){unsigned prev=word(self,12);if(word(prev,16)==self&&word(next,12)==self){word(prev,16)=next;word(word(self,16),12)=word(self,12);}word(self,12)=word(self,16)=0;}unsigned list=word(self,8),first=word(list,20);word(self,12)=list+4;word(self,16)=first;word(list+4,16)=self;word(word(self,16),12)=self;}}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));const unsigned patterns[6]={0,1,2,0x7fffffff,0x80000000,0xffffffff};unsigned cases=0,snapshots=0;
for(unsigned live=0;live<2;++live)for(unsigned alias=0;alias<2;++alias)for(unsigned next=0;next<2;++next)for(unsigned shape=0;shape<4;++shape)for(unsigned front=0;front<3;++front)for(unsigned age=0;age<6;++age)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;unsigned list=ptr(0x100),self=alias?list+4:ptr(0x400);memset(arena,fill?0xa9:0,sizeof(arena));word(list,20)=front==0?list+4:front==1?ptr(0x800):self;word(list,16)=ptr(0x900);word(list,44)=patterns[age];word(list,32)=patterns[(age+1)%6];word(list,36)=patterns[(age+2)%6];word(self,4)=patterns[(age+3)%6];word(self,8)=live?list:0;word(self,12)=ptr(0x600);word(self,16)=next?ptr(0x800):0;word(self,24)=patterns[age];word(ptr(0x600),16)=(shape&1)?ptr(0xa00):self;word(ptr(0x800),12)=(shape&2)?ptr(0xb00):self;memcpy(initial,arena,sizeof(arena));model(self);memcpy(expected,arena,sizeof(arena));
for(unsigned mode=0;mode<2;++mode){memcpy(arena,initial,sizeof(arena));(mode?SetLRU:((void(__fastcall*)(void*,void*))code))((void*)self,0);require(!memcmp(expected,arena,sizeof(arena)));++snapshots;}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_RESOURCE_SET_LRU_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
