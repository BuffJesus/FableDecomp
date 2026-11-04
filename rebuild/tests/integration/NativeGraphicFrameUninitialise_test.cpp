#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphic_frame_uninitialise_retail.h"
#include "graphic_frame_uninitialise_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall FrameUninitialise(void*,void*);
static unsigned arena[4096],initial[4096],images[5][4096],mode,count,step,snapshots,location,uninitMutation,pointeeMutation,heapMutation;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<5);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
extern "C" void __fastcall DataUninitialise(void* self,void*){require((unsigned)self==ptr(0x400));word(ptr(0x300),0)=1;word(ptr(0x100),0)=ptr(0x600);if(uninitMutation==1)word(ptr(0x400),12)=0;if(uninitMutation==2)word(ptr(0x400),12)=ptr(0xa00);if(uninitMutation==3)word(ptr(0x800),0)=1;snapshot();}
extern "C" void __fastcall PointeeDelete(void* self,void*){require((unsigned)self==ptr(0xc00));word(ptr(0x300),4)=1;word(ptr(0xc00),0)=0xdededede;if(pointeeMutation==1)word(ptr(0x400),12)=ptr(0xa00);if(pointeeMutation==2)word(ptr(0x400),12)=0;snapshot();}
void __cdecl operator delete(void* self){unsigned p=(unsigned)self;require(!p||p==ptr(0x400)||p==ptr(0x800)||p==ptr(0xa00));++word(ptr(0x300),8);word(ptr(0x300),12)=p;word(ptr(0x100),0)=ptr(0x600);if(heapMutation==1)word(ptr(0x400),12)=ptr(0xa00);if(heapMutation==2)word(ptr(0x400),8)=ptr(0xc00);snapshot();}
static void model(){unsigned data=word(ptr(0x100),0);if(data){if(--word(data,0)==0){if(*(unsigned char*)(data+26)&16)DataUninitialise((void*)data,0);unsigned info=word(data,12);if(info){--word(info,0);info=word(data,12);if(!word(info,0)){PointeeDelete((void*)word(info,8),0);operator delete((void*)word(data,12));}}word(data,8)=word(data,12)=0;operator delete((void*)data);}word(ptr(0x100),0)=0;}snapshot();}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[2]={(unsigned)&DataUninitialise,(unsigned)&operator delete};for(unsigned f=0;f<3;++f)*(unsigned*)(code+fixups[f].site)=targets[fixups[f].target]-(unsigned)(code+fixups[f].site+4);const unsigned refs[6]={0,1,2,3,0x80000000,0xffffffff};const unsigned flags[6]={0,1,4,16,17,255};unsigned cases=0;
for(unsigned live=0;live<2;++live)for(unsigned hasInfo=0;hasInfo<2;++hasInfo)for(unsigned ref=0;ref<6;++ref)for(unsigned infoRef=0;infoRef<6;++infoRef)for(unsigned flag=0;flag<6;++flag)for(uninitMutation=0;uninitMutation<4;++uninitMutation)for(pointeeMutation=0;pointeeMutation<3;++pointeeMutation)for(heapMutation=0;heapMutation<3;++heapMutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),0)=live?ptr(0x400):0;word(ptr(0x400),0)=refs[ref];word(ptr(0x400),8)=ptr(0xc00);word(ptr(0x400),12)=hasInfo?ptr(0x800):0;*(unsigned char*)ptr(0x41a)=(unsigned char)flags[flag];word(ptr(0x800),0)=refs[infoRef];word(ptr(0xa00),0)=1;word(ptr(0x800),4)=word(ptr(0xa00),4)=(unsigned)&PointeeDelete;word(ptr(0x800),8)=word(ptr(0xa00),8)=ptr(0xc00);memset((void*)ptr(0x300),0,16);memcpy(initial,arena,sizeof(arena));count=0;
for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=0;if(!mode)model();else{(mode==1?((void(__fastcall*)(void*,void*))code):FrameUninitialise)((void*)ptr(0x100),0);snapshot();require(step==count);}}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_GRAPHIC_FRAME_UNINITIALISE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
