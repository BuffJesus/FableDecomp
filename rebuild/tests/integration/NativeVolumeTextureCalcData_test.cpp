#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_volume_types.h"
#include "fable_memory_allocator.h"
#include "fable_d3d_format_info.h"
#include "volume_texture_calc_data_retail.h"
#include "volume_texture_calc_data_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall TextureCalcBytes(void*,void*);
extern "C" void* __fastcall ExecuteGetLevel(void*,void*,void*,long);
extern "C" void __fastcall ExecuteInit(void*,void*,void*);
extern "C" unsigned VolumeVTable[],BaseVTable[];
extern "C" void AbortBoundary(){abort();}
static unsigned arena[4096],initial[4096],images[40][4098];
static unsigned mode,step,count,snapshots,location,profile,mutation,levels,formatProfile;
static unsigned countCalls,getCalls,initCalls,adds,releases,queries,totalQueries,newVolume,requested;
static unsigned liveInner,liveOuter,outputReady;
static unsigned char* code;
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned address,unsigned offset){return *(unsigned*)(address+offset);}
static void require(bool value){if(!value)abort();}
static unsigned pattern(unsigned n){const unsigned values[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static void snapshot(){
 if(!mode){require(count<40);memcpy(images[count],arena,sizeof(arena));images[count][4096]=CVolume::TotalMemory;images[count++][4097]=g_FableAllocatorMemoryBaseline_013CAAB0;}
 else{require(step<count&&!memcmp(images[step],arena,sizeof(arena))&&images[step][4096]==CVolume::TotalMemory&&images[step][4097]==g_FableAllocatorMemoryBaseline_013CAAB0);++step;++snapshots;}
}
static void event(unsigned kind,unsigned argument,unsigned active){
 word(ptr(0x2000),0)=kind;word(ptr(0x2000),4)=argument;word(ptr(0x2000),8)=countCalls;word(ptr(0x2000),12)=getCalls;
 word(ptr(0x2000),16)=initCalls;word(ptr(0x2000),20)=adds;word(ptr(0x2000),24)=releases;word(ptr(0x2000),28)=queries;
 word(ptr(0x2000),32)=active?word(active,0):0;word(ptr(0x2000),36)=active?word(active,4):0;
 word(ptr(0x2000),40)=totalQueries;snapshot();
}
static unsigned long __stdcall levelCount(void* self){
 require(!countCalls&&!liveInner&&!liveOuter&&(unsigned)self==word(ptr(0x100),4)&&!word(ptr(0x100),8));++countCalls;
 if(mutation&2){word(ptr(0x100),4)=ptr(0x1400);word(ptr(0x100),8)=pattern(profile+5);}
 event(1,(unsigned)self,0);return levels;
}
static long __stdcall getLevel(void* self,unsigned index,void** out){
 require(liveOuter&&!liveInner&&!adds&&!releases&&!queries&&(unsigned)self==word(ptr(0x100),4)&&index==requested&&index==getCalls);++getCalls;
 newVolume=ptr(getCalls&1?0x800:0xa00);*out=(void*)newVolume;
 if(mutation&2){word(ptr(0x100),4)=ptr(getCalls&1?0x1400:0x400);word(ptr(0x100),8)=pattern(profile+getCalls);}
 event(2,index,0);return mutation&4?(long)0x80004005:0;
}
static unsigned long __stdcall addRef(void* self){
 require(liveInner&&liveOuter&&adds<2&&(unsigned)self==word(liveInner,4));++adds;
 if(adds==1){require((unsigned)self==newVolume);if(mutation&1)word(liveInner,4)=ptr(0xc00);}
 else{require(word(liveOuter,0)==(unsigned)VolumeVTable&&word(liveOuter,4)==(unsigned)self);outputReady=1;if(mutation&1)word(liveOuter,4)=ptr(0xc00);if(mutation&2)word(liveOuter,0)=pattern(profile+5);}
 if(mutation&4){CVolume::TotalMemory^=pattern(profile+6);g_FableAllocatorMemoryBaseline_013CAAB0+=pattern(profile+7);word(ptr(0x100),8)=pattern(profile+adds);}
 event(3,(unsigned)self,adds==1?liveInner:liveOuter);return pattern(profile+adds);
}
static long __stdcall description(void* self,D3DVOLUME_DESC* out){
 unsigned active=liveInner?liveInner:liveOuter;require(active&&adds==(liveInner?1:2)&&(unsigned)self==word(active,4)&&queries<(liveInner?4:6));
 unsigned current=queries++;++totalQueries;memset(out,0xa9,sizeof(*out));
 out->Format=(D3DFORMAT)((current&1)?retailFormats[((formatProfile+current/2)%46)*9]:0xdeadbeef);
 out->Width=pattern(profile+current+mutation);out->Height=pattern(profile+current+3);out->Depth=pattern(profile+current+5);
 if(mutation&1)word(active,4)=ptr(0xc00);
 if(mutation&2)word(ptr(0x100),8)+=pattern(profile+current);
 if(mutation&4){CVolume::TotalMemory+=pattern(profile+current+4);g_FableAllocatorMemoryBaseline_013CAAB0^=pattern(profile+current+6);}
 word(ptr(0x2000),44)=out->Width;word(ptr(0x2000),48)=out->Height;word(ptr(0x2000),52)=out->Depth;word(ptr(0x2000),56)=(unsigned)out->Format;
 event(4,(unsigned)self,active);return mutation&4?(long)0x80004005:0;
}
static unsigned long __stdcall release(void* self){
 unsigned active=liveInner?liveInner:liveOuter;require(active&&outputReady&&adds==2&&(unsigned)self==word(active,4)&&word(active,0)==(unsigned)VolumeVTable&&releases==(liveInner?0:1));++releases;
 if(mutation){word(ptr(0x100),8)^=pattern(profile+releases+3);word(active,4)=ptr(0xc00);word(active,0)=pattern(profile+7);}
 event(5,(unsigned)self,active);if(liveInner)liveInner=0;else liveOuter=0;return pattern(profile+releases);
}
static unsigned modelBytes(unsigned active){
 D3DVOLUME_DESC first,second;description((void*)word(active,4),&first);description((void*)word(active,4),&second);
 unsigned depth=0;for(unsigned row=0;row<46;++row)if(retailFormats[row*9]==(unsigned)second.Format){depth=retailFormats[row*9+2]/8;break;}
 return depth*first.Width*first.Height*first.Depth;
}
static void modelInit(void* volume){
 word(liveInner,4)=(unsigned)volume;addRef(volume);unsigned size=modelBytes(liveInner)+12;CVolume::TotalMemory+=size;
 size=modelBytes(liveInner)+12;g_FableAllocatorMemoryBaseline_013CAAB0+=size;
}
extern "C" void __fastcall ObserveInit(void* self,void*,void* volume){
 require(liveOuter&&!liveInner&&!adds&&!releases&&!queries&&(unsigned)volume==newVolume&&word((unsigned)self,0)==(unsigned)VolumeVTable&&!word((unsigned)self,4));
 liveInner=(unsigned)self;++initCalls;if(!mode)modelInit(volume);else(mode==1?((void(__fastcall*)(void*,void*,void*))(code+480)):ExecuteInit)(self,0,volume);
}
static unsigned modelGetLevel(void* self,void* output,long level){
 void* volume;getLevel((void*)word((unsigned)self,4),(unsigned)level,&volume);unsigned temp[2];temp[0]=(unsigned)BaseVTable;temp[0]=(unsigned)VolumeVTable;temp[1]=0;
 ObserveInit(temp,0,volume);word((unsigned)output,0)=(unsigned)BaseVTable;word((unsigned)output,0)=(unsigned)VolumeVTable;word((unsigned)output,4)=temp[1];addRef((void*)temp[1]);
 temp[0]=(unsigned)VolumeVTable;release((void*)temp[1]);temp[1]=0;temp[0]=(unsigned)BaseVTable;return (unsigned)output;
}
extern "C" void* __fastcall ObserveGetLevel(void* self,void*,void* output,long level){
 require((unsigned)self==ptr(0x100)&&!liveInner&&!liveOuter);liveOuter=(unsigned)output;requested=(unsigned)level;adds=releases=queries=outputReady=0;
 unsigned returned=!mode?modelGetLevel(self,output,level):(unsigned)(mode==1?((void*(__fastcall*)(void*,void*,void*,long))(code+128)):ExecuteGetLevel)(self,0,output,level);
 require(returned==(unsigned)output&&!liveInner&&liveOuter==(unsigned)output&&adds==2&&releases==1&&queries==4);return (void*)returned;
}
static void model(){
 word(ptr(0x100),8)=0;if(word(ptr(0x100),4)){
  long cached=(long)levelCount((void*)word(ptr(0x100),4));for(long level=0;level<cached;++level){
   unsigned volume[2];ObserveGetLevel((void*)ptr(0x100),0,volume,level);unsigned bytes=modelBytes((unsigned)volume);word(ptr(0x100),8)+=bytes;
   volume[0]=(unsigned)VolumeVTable;release((void*)volume[1]);volume[1]=0;volume[0]=(unsigned)BaseVTable;
  }
 }
}
int main(){
 require(!memcmp(retailFormats,GD3DFormats,sizeof(retailFormats)));code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 unsigned targets[8]={(unsigned)VolumeVTable,(unsigned)BaseVTable,(unsigned)&ObserveInit,(unsigned)&CVolume::TotalMemory,(unsigned)&g_FableAllocatorMemoryBaseline_013CAAB0,(unsigned)GD3DFormats,(unsigned)GD3DFormats+8,(unsigned)&ObserveGetLevel};
 for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=fixups[f].target>=0x80000000?targets[fixups[f].target-0x80000000]:(unsigned)(code+fixups[f].target);*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}
 unsigned textureTable[19]={0},volumeTable[9]={0};textureTable[13]=(unsigned)levelCount;textureTable[18]=(unsigned)getLevel;volumeTable[1]=(unsigned)addRef;volumeTable[2]=(unsigned)release;volumeTable[8]=(unsigned)description;
 const unsigned levelValues[5]={0,1,3,0x80000000,0xffffffff},profiles[4]={0,3,4,7};unsigned cases=0;
 for(formatProfile=0;formatProfile<46;++formatProfile)for(unsigned pd=0;pd<3;++pd)for(unsigned level=0;level<5;++level)for(unsigned p=0;p<4;++p)for(mutation=0;mutation<8;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
  profile=profiles[p];location=place?0x1800:0;levels=levelValues[level];memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=pd?ptr(pd==1?0x400:0x1400):0;word(ptr(0x100),8)=pattern(profile+1);word(ptr(0x100),12)=pattern(profile+2);
  word(ptr(0x400),0)=word(ptr(0x1400),0)=(unsigned)textureTable;word(ptr(0x800),0)=word(ptr(0xa00),0)=word(ptr(0xc00),0)=(unsigned)volumeTable;memset((void*)ptr(0x2000),0,64);memcpy(initial,arena,sizeof(arena));count=0;
  for(mode=0;mode<3;++mode){
   memcpy(arena,initial,sizeof(arena));CVolume::TotalMemory=pattern(profile);g_FableAllocatorMemoryBaseline_013CAAB0=pattern(profile+formatProfile);step=countCalls=getCalls=initCalls=adds=releases=queries=totalQueries=liveInner=liveOuter=outputReady=0;
   if(!mode)model();else(mode==1?((void(__fastcall*)(void*,void*))code):TextureCalcBytes)((void*)ptr(0x100),0);
   require(!liveInner&&!liveOuter&&countCalls==(pd?1:0)&&getCalls==((pd&&(long)levels>0)?levels:0)&&initCalls==getCalls&&totalQueries==getCalls*6);snapshot();if(mode)require(step==count);
  }++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_CALC_DATA_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
