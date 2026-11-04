#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "managed_texture_data_release_retail.h"
#include "managed_texture_data_release_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall TextureDataRelease(void*,void*);
static unsigned arena[4096],initial[4096],images[5][4096],mode,count,step,snapshots,location,pointeeMutation,heapMutation,lruMutation;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<5);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
extern "C" void __fastcall PointeeDelete(void* self,void*){require((unsigned)self==ptr(0xc00));word(ptr(0xc00),0)=0xdededede;word(ptr(0x300),0)=1;if(pointeeMutation==1)word(ptr(0x400),44)=ptr(0xa00);if(pointeeMutation==2){word(ptr(0x400),44)=0;word(ptr(0x400),56)=ptr(0xf00);}word(ptr(0x400),48)=ptr(0xc00);snapshot();}
void __cdecl operator delete(void* self){unsigned p=(unsigned)self;require(!p||p==ptr(0x800)||p==ptr(0xa00)||p==ptr(0xe00)||p==ptr(0xf00));++word(ptr(0x300),4);word(ptr(0x300),8)=p;if(heapMutation==1)word(ptr(0x400),44)=ptr(0xa00);if(heapMutation==2){word(ptr(0x400),40)=ptr(0xc00);word(ptr(0x400),56)=ptr(0xe00);}snapshot();}
extern "C" void __fastcall LRUBoundary(void* self,void*){require((unsigned)self==ptr(0x400));word(ptr(0x300),12)=1;if(lruMutation==1){word(ptr(0x400),48)=ptr(0xc00);*(unsigned char*)ptr(0x448)=1;}if(lruMutation==2){word(ptr(0x400),44)=ptr(0xa00);word(ptr(0x400),56)=ptr(0xf00);}snapshot();}
static void model(){unsigned data=ptr(0x400);*(unsigned char*)(data+72)=0;word(data,48)=0;unsigned info=word(data,44);if(info){--word(info,0);info=word(data,44);if(!word(info,0)){PointeeDelete((void*)word(info,8),0);operator delete((void*)word(data,44));}}word(data,44)=word(data,40)=0;unsigned header=word(data,56);word(data,56)=0;operator delete((void*)header);LRUBoundary((void*)data,0);snapshot();}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[2]={(unsigned)&operator delete,(unsigned)&LRUBoundary};for(unsigned f=0;f<3;++f)*(unsigned*)(code+fixups[f].site)=targets[fixups[f].target]-(unsigned)(code+fixups[f].site+4);const unsigned refs[6]={0,1,2,3,0x80000000,0xffffffff};const unsigned flags[4]={0,1,2,255};unsigned cases=0;
for(unsigned flag=0;flag<4;++flag)for(unsigned hasInfo=0;hasInfo<2;++hasInfo)for(unsigned hasHeader=0;hasHeader<2;++hasHeader)for(unsigned ref=0;ref<6;++ref)for(pointeeMutation=0;pointeeMutation<3;++pointeeMutation)for(heapMutation=0;heapMutation<3;++heapMutation)for(lruMutation=0;lruMutation<3;++lruMutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x400),40)=ptr(0xc00);word(ptr(0x400),44)=hasInfo?ptr(0x800):0;word(ptr(0x400),48)=ptr(0xc00);word(ptr(0x400),56)=hasHeader?ptr(0xe00):0;*(unsigned char*)ptr(0x448)=(unsigned char)flags[flag];word(ptr(0x800),0)=refs[ref];word(ptr(0x800),4)=(unsigned)&PointeeDelete;word(ptr(0x800),8)=ptr(0xc00);word(ptr(0xa00),0)=0x12345678;memset((void*)ptr(0x300),0,16);memcpy(initial,arena,sizeof(arena));count=0;
for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=0;if(!mode)model();else{(mode==1?((void(__fastcall*)(void*,void*))code):TextureDataRelease)((void*)ptr(0x400),0);snapshot();require(step==count);}}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_DATA_RELEASE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
