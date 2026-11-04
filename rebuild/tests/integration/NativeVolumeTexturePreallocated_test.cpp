#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_volume_types.h"
#include "fable_memory_allocator.h"
#include "volume_texture_preallocated_retail.h"
#include "volume_texture_preallocated_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" bool __fastcall VolumeTexturePreallocated(void*,void*,void*);
static unsigned arena[4096],initial[4096],images[16][4096],globals[16][3];
static unsigned mode,step,count,snapshots,location,profile,mutation,calls,queries,firstResult,initResult,systemCalls,waitCalls,incoming,calcCalls,calcQueries;
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned address,unsigned offset){return *(unsigned*)(address+offset);}
static unsigned pattern(unsigned n){const unsigned v[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return v[n&7];}
static void require(bool value){if(!value)abort();}
static void snapshot(){
 unsigned current[3]={(unsigned)g_FableMemoryAllocator_013CAA90,g_FableAllocatorMemoryBaseline_013CAAB0,CVolumeTexture::TotalMemory};
 if(!mode){require(count<16);memcpy(globals[count],current,sizeof(current));memcpy(images[count++],arena,sizeof(arena));}
 else{require(step<count&&!memcmp(globals[step],current,sizeof(current))&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}
}
static void event(unsigned kind,unsigned value){word(ptr(0x2000),0)=kind;word(ptr(0x2000),4)=value;word(ptr(0x2000),8)=calls;word(ptr(0x2000),12)=queries;word(ptr(0x2000),24)=systemCalls;word(ptr(0x2000),28)=waitCalls;snapshot();}
static unsigned long __stdcall addRef(void* volume){
 require((calls==0||calls==2)&&(unsigned)volume==word(ptr(0x100),4));++calls;
 if(mutation&1){word(ptr(0x100),4)=ptr((unsigned)volume==ptr(0x400)?0x800:0x400);word(ptr(0x100),0)^=0xffffffff;}
 event(1,(unsigned)volume);return pattern(profile);
}
static unsigned long __stdcall release(void* volume){
 require((calls==1||calls==3||calls==4)&&(unsigned)volume==word(ptr(0x100),4));++calls;
 if(calls==2||calls==4){
  if(mutation&2){word(ptr(0x100),12)=word(ptr(0x100),12)?0:ptr(0xc00);word(ptr(0x100),8)=pattern(profile+3);}
  event(2,(unsigned)volume);return calls==2?initResult:firstResult;
 }
 if(mutation){word(ptr(0x100),4)=pattern(profile+1);word(ptr(0x100),8)=pattern(profile+5);word(ptr(0x100),12)=pattern(profile+6);}
 event(3,(unsigned)volume);return pattern(profile+2);
}
extern "C" unsigned long __fastcall AllocationSize(void* self,void*,void* allocation){
 require((calcCalls||(calls==4&&firstResult==2))&&(unsigned)self==(unsigned)g_FableMemoryAllocator_013CAA90&&(unsigned)allocation==word(ptr(0x100),12));++queries;
 if(!calcCalls&&(mutation&4)){
  g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)ptr(queries==1?0x1000:0xe00);
  if(queries==1)word(ptr(0x100),12)=0;
  word(ptr(0x100),8)=pattern(profile+queries+1);word(ptr(0x100),4)=ptr(0x800);
  g_FableAllocatorMemoryBaseline_013CAAB0^=pattern(profile+6);CVolumeTexture::TotalMemory+=pattern(profile+3);
 }
 if(calcCalls){++calcQueries;
  if(mutation&1){word(ptr(0x100),12)=0;word(ptr(0x100),8)=pattern(profile+calcQueries+5);}
  if(mutation&2){word(ptr(0x100),4)=0;g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)ptr(0xe00);CVolumeTexture::TotalMemory^=pattern(profile+6);g_FableAllocatorMemoryBaseline_013CAAB0+=pattern(profile+7);}
 }
 word(ptr(0x2000),32)=calcCalls;word(ptr(0x2000),36)=calcQueries;event(4,(unsigned)allocation);return pattern(profile+queries);
}
static unsigned modelSize(){
 if(!word(ptr(0x100),4))return 0;
 unsigned data=word(ptr(0x100),12),size=data?AllocationSize(g_FableMemoryAllocator_013CAA90,0,(void*)data):word(ptr(0x100),8);
 return size+4;
}
static void modelRelease(){
 if(word(ptr(0x100),4)){
  addRef((void*)word(ptr(0x100),4));
  if(release((void*)word(ptr(0x100),4))==2){unsigned size=modelSize();g_FableAllocatorMemoryBaseline_013CAAB0-=size;size=modelSize();CVolumeTexture::TotalMemory-=size;}
  release((void*)word(ptr(0x100),4));word(ptr(0x100),4)=0;
 }
}
extern "C" void* __fastcall SystemGetter(){
 require(calls==2&&initResult==1&&!systemCalls);++systemCalls;
 if(mutation&4)word(ptr(0x100),4)=ptr(0x800);
 unsigned system=ptr(profile&1?0x1600:0x1000);event(5,system);return (void*)system;
}
extern "C" void __fastcall WaitResource(void* self,void*,void* resource,float timeout){
 unsigned bits;memcpy(&bits,&timeout,4);
 require(systemCalls==1&&!waitCalls&&calls==2&&(unsigned)resource==word(ptr(0x100),4)&&bits==0x3f800000);
 require((unsigned)self==ptr(profile&1?0x1a00:0x1400));++waitCalls;
 word(ptr(0x2000),16)=(unsigned)self;word(ptr(0x2000),20)=bits;
 if(mutation&4){word(ptr(0x100),4)=0;word(ptr(0x100),12)=pattern(profile+7);word(ptr(0x100),8)=pattern(profile+6);}
 event(6,(unsigned)resource);
}
static void modelUninitialise(){
 if(word(ptr(0x100),4)){
  addRef((void*)word(ptr(0x100),4));
  if(release((void*)word(ptr(0x100),4))==1){unsigned system=(unsigned)SystemGetter();unsigned display=word(system,0x60);WaitResource((void*)word(display,8),0,(void*)word(ptr(0x100),4),1.0f);}
  modelRelease();word(ptr(0x100),12)=0;word(ptr(0x100),4)=0;word(ptr(0x100),8)=0;
 }
}
extern "C" void __fastcall CalcBytes(void* self,void*){
 require((unsigned)self==ptr(0x100)&&!calcCalls&&word(ptr(0x100),4)==incoming);++calcCalls;word(ptr(0x100),8)=pattern(profile+4);
 if(mutation&1)word(ptr(0x100),12)=ptr(0xd00);
 if(mutation&2){CVolumeTexture::TotalMemory+=pattern(profile+6);g_FableAllocatorMemoryBaseline_013CAAB0^=pattern(profile+7);g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)ptr(0xe00);}
 if(mutation&4){word(ptr(0x100),4)=0;word(ptr(0x100),12)=pattern(profile+3);}
 word(ptr(0x2000),32)=calcCalls;event(7,incoming);
}
static bool modelPreallocated(){modelUninitialise();word(ptr(0x100),4)=incoming;CalcBytes((void*)ptr(0x100),0);unsigned size=modelSize();CVolumeTexture::TotalMemory+=size;size=modelSize();g_FableAllocatorMemoryBaseline_013CAAB0+=size;return true;}
int main(){
 require(!g_FableMemoryAllocator_013CAA90&&!g_FableAllocatorMemoryBaseline_013CAAB0&&!CVolumeTexture::TotalMemory);
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 unsigned targets[9]={(unsigned)&g_FableMemoryAllocator_013CAA90,(unsigned)&AllocationSize,(unsigned)&g_FableAllocatorMemoryBaseline_013CAAB0,(unsigned)&CVolumeTexture::TotalMemory,(unsigned)&SystemGetter,(unsigned)&WaitResource,(unsigned)(code+288),(unsigned)(code+160),(unsigned)&CalcBytes};
 for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=targets[fixups[f].target];*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}
 unsigned table[3]={0};table[1]=(unsigned)addRef;table[2]=(unsigned)release;const unsigned initResults[5]={0,1,2,0x80000001,0xffffffff};const unsigned results[6]={0,1,2,3,0x80000002,0xffffffff};unsigned cases=0;
 for(unsigned newPD=0;newPD<3;++newPD)for(unsigned init=0;init<5;++init)for(profile=0;profile<8;++profile)for(unsigned pd=0;pd<3;++pd)for(unsigned data=0;data<3;++data)for(unsigned result=0;result<6;++result)for(mutation=0;mutation<8;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
  location=place?0x1800:0;incoming=newPD?ptr(newPD==1?0x1200:0x1400):0;firstResult=results[result];initResult=initResults[init];memset(arena,fill?0xa9:0,sizeof(arena));
  word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=pd?ptr(pd==1?0x400:0x800):0;word(ptr(0x100),8)=pattern(profile+1);word(ptr(0x100),12)=data?ptr(data==1?0xc00:0xd00):0;
  word(ptr(0x400),0)=word(ptr(0x800),0)=(unsigned)table;word(ptr(0x1000),0x60)=ptr(0x1200);word(ptr(0x1200),8)=ptr(0x1400);word(ptr(0x1600),0x60)=ptr(0x1800);word(ptr(0x1800),8)=ptr(0x1a00);memset((void*)ptr(0x2000),0,44);memcpy(initial,arena,sizeof(arena));count=0;
  for(mode=0;mode<3;++mode){
   memcpy(arena,initial,sizeof(arena));g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)ptr(profile&1?0x1000:0xe00);g_FableAllocatorMemoryBaseline_013CAAB0=pattern(profile+2);CVolumeTexture::TotalMemory=pattern(profile+4);step=calls=queries=systemCalls=waitCalls=calcCalls=calcQueries=0;
   bool result=!mode?modelPreallocated():(mode==1?((bool(__fastcall*)(void*,void*,void*))code):VolumeTexturePreallocated)((void*)ptr(0x100),0,(void*)incoming);require(result&&calcCalls==1);word(ptr(0x2000),40)=result;
   snapshot();if(mode)require(step==count&&calls==(pd?((initResult==1&&(mutation&4))?2:5):0)&&systemCalls==((pd&&initResult==1)?1:0)&&waitCalls==systemCalls);
  }++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_PREALLOCATED_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
