#include <stdio.h>
#include <stdlib.h>
extern "C" void AbortBoundary(){abort();}
#include <string.h>
#include "fable_memory_allocator.h"
#include "volume_texture_sizeof_retail.h"
#include "volume_texture_sizeof_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned long __fastcall VolumeTextureSizeof(void*,void*);
static unsigned arena[4096],initial[4096],images[2][4096],allocatorImages[2];
static unsigned mode,step,count,snapshots,location,profile,byteProfile,mutation,calls,allocationArg,allocatorArg;
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned address,unsigned offset){return *(unsigned*)(address+offset);}
static unsigned pattern(unsigned n){const unsigned v[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return v[n&7];}
static void require(bool value){if(!value)abort();}
static void snapshot(){
 if(!mode){require(count<2);allocatorImages[count]=(unsigned)g_FableMemoryAllocator_013CAA90;memcpy(images[count++],arena,sizeof(arena));}
 else{require(step<count&&allocatorImages[step]==(unsigned)g_FableMemoryAllocator_013CAA90&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}
}
extern "C" unsigned long __fastcall AllocationSize(void* self,void*,void* allocation){
 require(!calls&&(unsigned)self==allocatorArg&&(unsigned)allocation==allocationArg);++calls;
 word(ptr(0x2000),0)=calls;word(ptr(0x2000),4)=(unsigned)self;word(ptr(0x2000),8)=(unsigned)allocation;
 if(mutation&1){word(ptr(0x100),0)^=0xffffffff;word(ptr(0x100),4)=0;word(ptr(0x100),8)=pattern(byteProfile+3);word(ptr(0x100),12)=ptr(0x800);}
 if(mutation&2)g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)pattern(profile+1);
 snapshot();return pattern(profile);
}
static unsigned model(){
 if(!word(ptr(0x100),4))return 0;
 unsigned data=word(ptr(0x100),12);
 unsigned size=data?AllocationSize(g_FableMemoryAllocator_013CAA90,0,(void*)data):word(ptr(0x100),8);
 return size+4;
}
int main(){
 require(g_FableMemoryAllocator_013CAA90==0);
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 unsigned targets[2]={(unsigned)&g_FableMemoryAllocator_013CAA90,(unsigned)&AllocationSize};
 for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=targets[fixups[f].target];*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}
 unsigned cases=0;const unsigned dataValues[4]={0,0x400,0x800,0xdeadbeef};
 for(profile=0;profile<8;++profile)for(byteProfile=0;byteProfile<8;++byteProfile)for(unsigned pd=0;pd<3;++pd)for(unsigned data=0;data<4;++data)for(unsigned allocator=0;allocator<2;++allocator)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
  location=place?0x1800:0;allocatorArg=ptr(allocator?0xc00:0xa00);allocationArg=data<3?(data?ptr(dataValues[data]):0):dataValues[data];
  memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=pd?(pd==1?ptr(0x400):0xffffffff):0;
  word(ptr(0x100),8)=pattern(byteProfile);word(ptr(0x100),12)=allocationArg;memset((void*)ptr(0x2000),0,16);memcpy(initial,arena,sizeof(arena));count=0;
  for(mode=0;mode<3;++mode){
   memcpy(arena,initial,sizeof(arena));g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)allocatorArg;step=calls=0;
   unsigned result=!mode?model():(mode==1?((unsigned long(__fastcall*)(void*,void*))code):VolumeTextureSizeof)((void*)ptr(0x100),0);
   word(ptr(0x2000),12)=result;snapshot();if(mode)require(step==count&&calls==((pd&&data)?1:0));
  }++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_SIZEOF_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
