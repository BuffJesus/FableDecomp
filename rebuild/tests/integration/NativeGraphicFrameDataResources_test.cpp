#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphic_frame_data_resources_retail.h"
#include "graphic_frame_data_resources_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void* __fastcall FrameDataConstructor(void*,void*,const void*);
extern "C" unsigned FrameVTable[],ManagedVTable[];
extern "C" void AbortBoundary(){abort();}
static unsigned arena[16384],initial[16384],images[9][16384];
static unsigned mode,count,step,snapshots,location,baseCalls,mutation,frameSize,managedSize,bankMutation;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<9);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
extern "C" void* __fastcall ResourceCtor(void* self,void*){require((unsigned)self==ptr(0x100));word((unsigned)self,0)=0x11223344;word((unsigned)self,4)=1;for(unsigned o=8;o<28;o+=4)word((unsigned)self,o)=0;word(ptr(0x3000),0)=1;snapshot();return self;}
extern "C" void* __fastcall BaseCtor(void* self,void*){unsigned p=(unsigned)self;require(p==ptr(baseCalls?0x128:0x120));++baseCalls;word(p,0)=0x55667788;word(ptr(0x3000),4)=baseCalls;if(baseCalls==2){if(mutation&1)word(ptr(0x100),28)=ptr(0x2400);if(mutation&2)word(ptr(0x100),44)=ptr(0x2500);if(mutation&4)word(ptr(0x1000),12)=ptr(0x2200);}snapshot();return self;}
extern "C" unsigned __fastcall GraphicSize(const void* self,void*){require((unsigned)self==ptr(0x2400));word(ptr(0x3000),8)=1;if(mutation&8)word(ptr(0x100),44)=ptr(0x2500);if(bankMutation)word(ptr(0x1000),12)=ptr(0x2200);snapshot();return frameSize;}
extern "C" unsigned __fastcall ManagedSize(const void* self,void*){require((unsigned)self==ptr(0x128));word(ptr(0x3000),12)=1;if(bankMutation)word(ptr(0x1000),12)=ptr(0x2200);snapshot();return managedSize;}
static void modelSetSize(unsigned self,unsigned size){unsigned list=word(self,8);if(list){word(list,36)-=word(self,20);word(list,36)+=size;}word(self,20)=size;}
static void modelAdd(unsigned bank,unsigned resource){unsigned list=bank+4;word(resource,8)=list;word(resource,24)=word(list,44);word(resource,12)=word(list,16);word(resource,16)=list+4;word(word(resource,12),16)=resource;word(word(resource,16),12)=resource;++word(list,32);word(list,36)+=word(resource,20);}
static void model(){unsigned self=ptr(0x100);ResourceCtor((void*)self,0);word(self,0)=(unsigned)FrameVTable;word(self,28)=0;BaseCtor((void*)(self+32),0);word(self,32)=(unsigned)ManagedVTable;word(self,36)=0;BaseCtor((void*)(self+40),0);word(self,40)=(unsigned)ManagedVTable;word(self,44)=0;word(self,48)=word(self,52)=0;word(self,64)=ptr(0x1000);*(unsigned char*)(self+56)=255;*(unsigned char*)(self+58)=127;word(self,60)=word(word(word(ptr(0x1000),12),32),0x194)-1;unsigned size=68+4+(word(self,28)?GraphicSize((void*)word(self,28),0):0);if(word(self,44))size+=ManagedSize((void*)(self+40),0);modelSetSize(self,size);modelAdd(word(word(ptr(0x1000),12),32)+0x164,self);snapshot();}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[8]={(unsigned)&ResourceCtor,(unsigned)&BaseCtor,(unsigned)FrameVTable,(unsigned)ManagedVTable,(unsigned)&GraphicSize,(unsigned)&ManagedSize,(unsigned)(code+384),(unsigned)(code+256)};
for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned t=targets[fixups[f].target];*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?t-(unsigned)(code+fixups[f].site+4):t;}
const unsigned patterns[5]={0,1,0x7fffffff,0x80000000,0xffffffff};unsigned cases=0;
for(mutation=0;mutation<16;++mutation)for(bankMutation=0;bankMutation<2;++bankMutation)for(unsigned age=0;age<5;++age)for(unsigned fs=0;fs<5;++fs)for(unsigned ms=0;ms<5;++ms)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x4000:0;frameSize=patterns[fs];managedSize=patterns[ms];memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x1000),12)=ptr(0x2000);word(ptr(0x2000),32)=ptr(0x400);word(ptr(0x2200),32)=ptr(0x800);word(ptr(0x400),0x194)=patterns[age];word(ptr(0x800),0x194)=patterns[(age+1)%5];for(unsigned bk=0;bk<2;++bk){unsigned bank=ptr(bk?0x800:0x400);word(bank,0x178)=ptr(bk?0x2900:0x2800);word(bank,0x188)=patterns[(age+2)%5];word(bank,0x18c)=patterns[(age+3)%5];}memset((void*)ptr(0x3000),0,24);memcpy(initial,arena,sizeof(arena));count=0;
for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));baseCalls=step=0;if(!mode)model();else{void* result=(mode==1?((void*(__fastcall*)(void*,void*,const void*))code):FrameDataConstructor)((void*)ptr(0x100),0,(void*)ptr(0x1000));require((unsigned)result==ptr(0x100));snapshot();require(step==count);}}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_GRAPHIC_FRAME_DATA_RESOURCES_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
