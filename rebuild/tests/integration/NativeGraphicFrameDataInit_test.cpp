#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphic_frame_data_init_retail.h"
#include "graphic_frame_data_init_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void* __fastcall FrameDataConstructor(void*,void*,const void*);
extern "C" unsigned FrameVTable[],ManagedVTable[],ResourceVTable[],BaseVTable[];
extern "C" void AbortBoundary(){abort();}
static unsigned arena[16384],initial[16384],expected[16384],location;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void model(unsigned bank){unsigned self=ptr(0x100),list=bank+0x168,head=list+4;word(self,0)=(unsigned)FrameVTable;word(self,4)=1;for(unsigned o=8;o<28;o+=4)word(self,o)=0;word(self,28)=0;word(self,32)=word(self,40)=(unsigned)ManagedVTable;word(self,36)=word(self,44)=word(self,48)=word(self,52)=0;word(self,64)=ptr(0x1000);*(unsigned char*)(self+56)=255;*(unsigned char*)(self+58)=127;word(self,60)=word(list,44)-1;word(self,20)=72;word(self,8)=list;word(self,24)=word(list,44);word(self,12)=word(head,12);word(self,16)=head;word(word(self,12),16)=self;word(word(self,16),12)=self;++word(list,32);word(list,36)+=word(self,20);}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[8]={(unsigned)(code+448),(unsigned)(code+496),(unsigned)FrameVTable,(unsigned)ManagedVTable,(unsigned)&AbortBoundary,(unsigned)&AbortBoundary,(unsigned)(code+384),(unsigned)(code+256)};
for(unsigned f=0;f<10;++f){unsigned t=targets[fixups[f].target];*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?t-(unsigned)(code+fixups[f].site+4):t;}
*(unsigned*)(code+448+13)=(unsigned)ResourceVTable;*(unsigned*)(code+496+4)=(unsigned)BaseVTable;const unsigned patterns[6]={0,1,2,0x7fffffff,0x80000000,0xffffffff};unsigned cases=0,snapshots=0;
for(unsigned mip=0;mip<256;++mip)for(unsigned age=0;age<6;++age)for(unsigned bankID=0;bankID<2;++bankID)for(unsigned empty=0;empty<2;++empty)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x4000:0;memset(arena,fill?0xa9:0,sizeof(arena));unsigned bank=ptr(bankID?0x800:0x400),list=bank+0x168,head=list+4;word(ptr(0x1000),12)=ptr(0x2000);word(ptr(0x2000),32)=bank;word(list,44)=patterns[age];word(list,32)=patterns[(age+1)%6];word(list,36)=patterns[(age+2)%6];word(head,12)=empty?head:ptr(0x2800);word(head,16)=empty?head:ptr(0x2900);*(unsigned char*)ptr(0x139)=(unsigned char)mip;memcpy(initial,arena,sizeof(arena));model(bank);memcpy(expected,arena,sizeof(arena));
for(unsigned mode=0;mode<2;++mode){memcpy(arena,initial,sizeof(arena));void* result=(mode?FrameDataConstructor:((void*(__fastcall*)(void*,void*,const void*))code))((void*)ptr(0x100),0,(void*)ptr(0x1000));require((unsigned)result==ptr(0x100)&&!memcmp(expected,arena,sizeof(arena)));++snapshots;}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_GRAPHIC_FRAME_DATA_INIT_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
