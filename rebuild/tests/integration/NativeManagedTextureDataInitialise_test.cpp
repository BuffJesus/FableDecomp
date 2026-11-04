#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "managed_texture_data_initialise_retail.h"
#include "managed_texture_data_initialise_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall ManagedDataInitialise(void*,void*,unsigned long,unsigned long);
static unsigned arena[4096],initial[4096],images[3][4096],mode,step,count,snapshots,location,profile,mutation,allocation,pool,allocCalls,resetCalls,heap;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void snapshot(){if(!mode){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
extern "C" void* Allocate(unsigned size){require(size==8&&!allocCalls&&!resetCalls);++allocCalls;word(ptr(0x2000),0)=1;word(ptr(0x2000),4)=size;word(ptr(0x2000),8)=heap;if(mutation&1){word(ptr(0x100),0x20)=pattern(profile+2);word(ptr(0x100),0x24)=pattern(profile+3);if(heap){word(heap,0)=pattern(profile+4);word(heap,4)=pattern(profile+5);}}snapshot();return (void*)heap;}
void* operator new(unsigned size){return Allocate(size);}
extern "C" void __fastcall ResetTexture(void* self,void*,void* texture){require((unsigned)self==ptr(0x120)&&(unsigned)texture==heap&&allocCalls==1&&!resetCalls);++resetCalls;if(heap)require(!word(heap,0)&&!word(heap,4));word(ptr(0x2000),0)=2;word(ptr(0x2000),12)=(unsigned)self;word(ptr(0x2000),16)=(unsigned)texture;word(ptr(0x120),0)=(unsigned)texture;word(ptr(0x120),4)=heap?ptr(0x900):0;if(mutation&2){word(ptr(0x100),0x40)=pattern(profile+6);word(ptr(0x100),0x44)=pattern(profile+7);word(ptr(0x100),0x48)^=pattern(profile+2);if(heap){word(heap,0)=pattern(profile+1);word(heap,4)=pattern(profile+2);}}snapshot();}
static void model(){void* result=Allocate(8);if(result){word((unsigned)result,0)=0;word((unsigned)result,4)=0;}ResetTexture((void*)ptr(0x120),0,result);word(ptr(0x100),0x40)=pattern(profile);word(ptr(0x100),0x44)=pool;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[2]={(unsigned)Allocate,(unsigned)ResetTexture};for(unsigned f=0;f<sizeof(fixups)/sizeof(*fixups);++f){require(fixups[f].kind==20&&fixups[f].target<2);*(unsigned*)(code+fixups[f].site)=targets[fixups[f].target]-(unsigned)(code+fixups[f].site+4);}unsigned cases=0;
for(profile=0;profile<8;++profile)for(allocation=0;allocation<3;++allocation)for(unsigned pp=0;pp<8;++pp)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;pool=pattern(pp);heap=allocation?ptr(allocation==1?0x600:0x800):0;memset(arena,fill?0xa9:0,sizeof(arena));for(unsigned i=0;i<19;++i)word(ptr(0x100),i*4)=pattern(profile+i);memset((void*)ptr(0x2000),0,24);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=allocCalls=resetCalls=0;if(!mode)model();else(mode==1?((void(__fastcall*)(void*,void*,unsigned long,unsigned long))code):ManagedDataInitialise)((void*)ptr(0x100),0,pattern(profile),pool);require(allocCalls==1&&resetCalls==1&&word(ptr(0x100),0x40)==pattern(profile)&&word(ptr(0x100),0x44)==pool);snapshot();if(mode)require(step==count);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_DATA_INITIALISE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
