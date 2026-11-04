#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_volume_types.h"
#include "fable_d3d_format_info.h"
#include "volume_bank_load_data_retail.h"
#include "volume_bank_load_data_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" bool __fastcall VolumeBankLoad(void*,void*,void*,void*,void*,bool,unsigned long,bool);
extern "C" unsigned VolumeVTable[],BaseVTable[];
extern "C" void AbortBoundary(){fprintf(stderr,"unexpected abort-only boundary\n");fflush(stderr);abort();}
static unsigned arena[4096],initial[4096],images[128][4096];
static unsigned mode,step,count,snapshots,location,profile,formatProfile,mutation,infoPresent,refProfile,poolProfile,entered,left;
static unsigned getCalls,inits,adds,releases,descs,locks,datas,unlocks,deletes,heaps,phase,liveTemp,liveRoot,liveBox;
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned address,unsigned offset){return *(unsigned*)(address+offset);}
static void requireAt(bool value,unsigned line){if(!value){fprintf(stderr,"failure line=%u mode=%u step=%u count=%u profile=%u format=%u mutation=%u info=%u refs=%u get=%u init=%u add=%u release=%u desc=%u lock=%u data=%u unlock=%u delete=%u heap=%u phase=%u\n",line,mode,step,count,profile,formatProfile,mutation,infoPresent,refProfile,getCalls,inits,adds,releases,descs,locks,datas,unlocks,deletes,heaps,phase);fflush(stderr);abort();}}
#define require(value) requireAt((value),__LINE__)
static unsigned pattern(unsigned n){const unsigned values[4]={0,1,2,0xffffffff};return values[n&3];}
static void snapshot(){
 if(!mode){require(count<128);memcpy(images[count++],arena,sizeof(arena));}
 else{require(step<count);if(memcmp(images[step],arena,sizeof(arena))){for(unsigned i=0;i<4096;++i)if(images[step][i]!=arena[i]){fprintf(stderr,"difference offset=%x model=%x actual=%x\n",i*4,images[step][i],arena[i]);fflush(stderr);break;}}require(!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}
}
static void event(unsigned id,unsigned value){
 unsigned log=ptr(0x2800);word(log,0)=id;word(log,4)=value;
 word(log,8)=getCalls;word(log,12)=inits;word(log,16)=adds;word(log,20)=releases;word(log,24)=descs;
 word(log,28)=locks;word(log,32)=datas;word(log,36)=unlocks;word(log,40)=deletes;word(log,44)=heaps;
 if(liveTemp){word(log,48)=word(liveTemp,0);word(log,52)=word(liveTemp,4);}
 if(liveRoot){word(log,56)=word(liveRoot,0);word(log,60)=word(liveRoot,4);}
 word(log,80)=entered;word(log,84)=left;
 snapshot();
}
static long __stdcall getLevel(void* texture,unsigned level,void** output){
 require((unsigned)texture==ptr(0x800)&&level==0&&getCalls<2&&!phase);
 ++getCalls;phase=1;*output=(void*)ptr(getCalls==1?0xa00:0xc00);
 if(mutation==1&&getCalls==1){word(ptr(0x200),0x28)=ptr(0x700);word(ptr(0x200),0x2c)=ptr(0x580);}
 event(1,(unsigned)*output);return mutation==3?(long)0x80004005:0;
}
extern "C" void __fastcall InitVolume(void* self,void*,void* volume){
 require(phase&&!liveTemp&&word((unsigned)self,0)==(unsigned)VolumeVTable&&!word((unsigned)self,4));
 ++inits;liveTemp=(unsigned)self;word(liveTemp,4)=(unsigned)volume;event(2,(unsigned)volume);
}
static long __stdcall addRef(void* volume){
 require(phase&&liveTemp&&word(liveTemp,4)==(unsigned)volume);
 ++adds;++word((unsigned)volume,4);event(3,(unsigned)volume);return (long)pattern(profile);
}
static long __stdcall release(void* volume){
 require((unsigned)volume==ptr(0xa00)||(unsigned)volume==ptr(0xc00));
 ++releases;--word((unsigned)volume,4);event(4,(unsigned)volume);
 if(phase){phase=liveTemp=0;}else if(unlocks)liveRoot=0;
 return (long)pattern(profile+1);
}
static long __stdcall getDesc(void* volume,D3DVOLUME_DESC* desc){
 require((unsigned)volume==ptr(0xa00)||(unsigned)volume==ptr(0xc00));++descs;
 memset(desc,0xa9,sizeof(*desc));desc->Format=(D3DFORMAT)retailFormats[formatProfile*9];
 desc->Width=(profile+((mutation==3)?descs:0))%4;
 desc->Height=(profile+1+((mutation==3)?descs:0))%4;
 desc->Depth=(profile+2+((mutation==3)?descs:0))%4;
 if(mutation==3&&liveRoot){word(liveRoot,4)=ptr(descs&1?0xc00:0xa00);if(liveBox){word(liveBox,12)=80;word(liveBox,16)=320;}}
 word(ptr(0x2800),64)=desc->Width;word(ptr(0x2800),68)=desc->Height;word(ptr(0x2800),72)=desc->Depth;
 event(5,(unsigned)volume);return mutation==3?(long)0x80004005:0;
}
extern "C" CVolumeLockedBox* __fastcall LockVolume(void* self,void*,CVolumeLockedBox* output,unsigned long flags){
 require(!locks&&flags==0&&!phase);++locks;liveRoot=(unsigned)self;liveBox=(unsigned)output;
 require(word(liveRoot,0)==(unsigned)VolumeVTable&&word(liveRoot,4)==ptr(0xa00));
 for(unsigned w=0;w<3;++w)word(liveBox,w*4)=pattern(profile+w);
 word(liveBox,12)=64;word(liveBox,16)=256;word(liveBox,20)=ptr(0x1800);
 event(6,flags);return output;
}
static void* onEnter(void* self){
 require((unsigned)self==ptr(0x600)&&locks==1&&!datas);++datas;
 if(mutation==1&&infoPresent)word(ptr(0x500),0)=1;
 if(mutation==2){word(liveBox,20)=ptr(0x2200);word(liveBox,12)=80;word(liveBox,16)=320;word(liveRoot,4)=ptr(0xc00);}
 unsigned data=ptr(mutation==2?0x1400:0x1000);word(ptr(0x600),8)=data;event(7,data);return (void*)data;
}
static void __stdcall enterSection(void* section){
 require((unsigned)section==ptr(0x44c)&&!entered&&!left);entered=1;
 if(mutation==1){word(ptr(0x600),0x28)=ptr(0x900);word(ptr(0x600),0x20)=0xffffffff;}
 onEnter((void*)ptr(0x600));
}
static void __stdcall leaveSection(void* section){
 require((unsigned)section==ptr(0x44c)&&entered&&!left);left=1;
 if(mutation==2)word(ptr(0x600),8)=ptr(0x1000);
 if(mutation==3)*(unsigned char*)ptr(0x645)=0x80;
 event(11,(unsigned)section);
}
static int __stdcall trySection(void*){require(false);return 0;}
extern "C"{unsigned EnterIAT=(unsigned)&enterSection,LeaveIAT=(unsigned)&leaveSection,TryIAT=(unsigned)&trySection;}
static void* modelAsyncData(){
 unsigned self=ptr(0x600),section=word(self,0x28)+0x14c;enterSection((void*)section);
 unsigned pool=word(self,0x1c),handle=word(self,0x20);
 if(pool&&handle!=0xffffffff&&!*(unsigned char*)(self+0x45)){
  unsigned header=word(word(pool,0x438),handle*4);
  if(!*(unsigned char*)(header+15)){*(unsigned char*)(header+15)=1;++word(pool,24);}
  *(unsigned char*)(self+0x45)=1;
 }
 unsigned data=word(self,8);leaveSection((void*)section);return (void*)data;
}
static long __stdcall unlock(void* volume){
 require(getCalls==2&&!phase&&liveRoot&&(unsigned)volume==word(liveRoot,4)&&!unlocks);
 ++unlocks;event(8,(unsigned)volume);return (long)0x80004005;
}
static void __fastcall deleteData(void* data){
 require((unsigned)data==ptr(0x550)&&infoPresent&&!deletes&&!liveRoot&&!phase&&word(ptr(0x500),0)==0);
 ++deletes;word(ptr(0x550),0)^=0xffffffff;word(ptr(0x500),8)=ptr(0x560);event(9,(unsigned)data);
}
extern "C" void __cdecl HeapDelete(void* data){
 require((unsigned)data==ptr(0x500)&&deletes==1&&!heaps);++heaps;
 memset(data,0xe7,12);event(10,(unsigned)data);
}
void operator delete(void* data){HeapDelete(data);}
static void modelGetLevel(unsigned texture,unsigned output){
 void* volume;getLevel((void*)word(texture,4),0,&volume);
 unsigned temp[2]={(unsigned)VolumeVTable,0};InitVolume(temp,0,volume);
 word(output,0)=(unsigned)VolumeVTable;word(output,4)=temp[1];if(temp[1])addRef((void*)temp[1]);
 temp[0]=(unsigned)VolumeVTable;if(temp[1]){release((void*)temp[1]);temp[1]=0;}temp[0]=(unsigned)BaseVTable;
}
static D3DVOLUME_DESC modelDesc(unsigned volume){D3DVOLUME_DESC result;getDesc((void*)word(volume,4),&result);return result;}
static bool model(){
 unsigned managed=word(ptr(0x100),4),texture=word(managed,0x28),info=word(managed,0x2c);
 if(info)++word(info,0);
 unsigned volume[2];modelGetLevel(texture,(unsigned)volume);
 modelDesc((unsigned)volume);modelDesc((unsigned)volume); // Byte-length result is unused; both calls remain observable.
 CVolumeLockedBox box;LockVolume(volume,0,&box,0);unsigned char* slice=box.PMem;
 unsigned char* data=(unsigned char*)modelAsyncData();
 D3DVOLUME_DESC format=modelDesc((unsigned)volume);unsigned byteDepth=0;
 for(unsigned row=0;row<46;++row)if(retailFormats[row*9]==(unsigned)format.Format){byteDepth=retailFormats[row*9+2]/8;break;}
 for(long z=0;z<(long)modelDesc((unsigned)volume).Depth;++z){
  for(long y=0;y<(long)modelDesc((unsigned)volume).Height;++y){
   unsigned bytes=modelDesc((unsigned)volume).Width*byteDepth;
   // Independent byte loop also checks native REP MOVSD/MOVSB tails.
   unsigned char* target=slice+y*box.RowPitch;for(unsigned b=0;b<bytes;++b)target[b]=data[b];
   data+=modelDesc((unsigned)volume).Width*byteDepth;
  }slice+=box.SlicePitch;
 }
 unsigned second[2];modelGetLevel(texture,(unsigned)second); // Swizzle itself has no observable effect.
 second[0]=(unsigned)VolumeVTable;if(second[1]){release((void*)second[1]);second[1]=0;}second[0]=(unsigned)BaseVTable;
 unlock((void*)volume[1]);volume[0]=(unsigned)VolumeVTable;
 if(volume[1]){release((void*)volume[1]);volume[1]=0;}volume[0]=(unsigned)BaseVTable;
 liveBox=0;
 if(info){--word(info,0);if(!word(info,0)){deleteData((void*)word(info,8));HeapDelete((void*)info);}}
 return true;
}
int main(){
 require(!memcmp(retailFormats,GD3DFormats,sizeof(retailFormats)));
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 unsigned targets[11]={(unsigned)VolumeVTable,(unsigned)BaseVTable,(unsigned)&InitVolume,(unsigned)&LockVolume,0,(unsigned)&HeapDelete,(unsigned)GD3DFormats,(unsigned)GD3DFormats+8,(unsigned)&EnterIAT,(unsigned)&TryIAT,(unsigned)&LeaveIAT};
 for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=fixups[f].target>=0x80000000?targets[fixups[f].target-0x80000000]:(unsigned)(code+fixups[f].target);*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}
 unsigned textureTable[19]={0},volumeTable[11]={0};textureTable[18]=(unsigned)getLevel;volumeTable[1]=(unsigned)addRef;volumeTable[2]=(unsigned)release;volumeTable[8]=(unsigned)getDesc;volumeTable[10]=(unsigned)unlock;unsigned cases=0;
 for(poolProfile=0;poolProfile<4;++poolProfile)for(formatProfile=0;formatProfile<46;++formatProfile)for(profile=0;profile<4;++profile)for(mutation=0;mutation<4;++mutation)for(infoPresent=0;infoPresent<2;++infoPresent)for(refProfile=0;refProfile<4;++refProfile)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
  location=place?0x1000:0;memset(arena,fill?0xa9:0,sizeof(arena));
  word(ptr(0x100),4)=ptr(0x200);word(ptr(0x200),0x28)=ptr(0x400);word(ptr(0x200),0x2c)=infoPresent?ptr(0x500):0;
  word(ptr(0x400),4)=ptr(0x800);word(ptr(0x800),0)=(unsigned)textureTable;
  word(ptr(0xa00),0)=word(ptr(0xc00),0)=(unsigned)volumeTable;word(ptr(0xa00),4)=pattern(profile);word(ptr(0xc00),4)=pattern(profile+1);
  word(ptr(0x500),0)=pattern(refProfile);word(ptr(0x500),4)=(unsigned)deleteData;word(ptr(0x500),8)=ptr(0x550);
  for(unsigned b=0;b<0x800;++b)*(unsigned char*)ptr(0x1000+b)=(unsigned char)(b*13+profile*37);
  word(ptr(0x600),8)=ptr(0x1000);word(ptr(0x600),0x28)=ptr(0x300);
  word(ptr(0x600),0x1c)=poolProfile?ptr(0xd00):0;word(ptr(0x600),0x20)=poolProfile==2?7:0;
  *(unsigned char*)ptr(0x645)=poolProfile==3?0x80:0;
  word(ptr(0xd00),24)=pattern(refProfile);word(ptr(0xd00),0x438)=ptr(0xe00);
  for(unsigned h=0;h<8;++h)word(ptr(0xe00),h*4)=ptr(0xe40);
  *(unsigned char*)ptr(0xe4f)=poolProfile==2?0x80:0;
  memset((void*)ptr(0x2800),0,96);memcpy(initial,arena,sizeof(arena));count=0;
  for(mode=0;mode<3;++mode){
   memcpy(arena,initial,sizeof(arena));step=getCalls=inits=adds=releases=descs=locks=datas=unlocks=deletes=heaps=phase=liveTemp=liveRoot=liveBox=entered=left=0;
   bool result=!mode?model():(mode==1?((bool(__fastcall*)(void*,void*,void*,void*,void*,bool,unsigned long,bool))code):VolumeBankLoad)((void*)ptr(0x50),0,(void*)ptr(0x100),(void*)ptr(0x600),(void*)ptr(0x680),(profile&1)!=0,pattern(profile),(profile&2)!=0);
   liveRoot=liveTemp=liveBox=0;word(ptr(0x2800),76)=result;snapshot();
   if(mode)require(step==count&&result&&getCalls==2&&inits==2&&adds==2&&releases==4&&locks==1&&datas==1&&unlocks==1&&entered&&left);
  }++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_BANK_LOAD_DATA_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
