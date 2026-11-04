#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "volume_texture_calc_bytes_retail.h"
#include "volume_texture_calc_bytes_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall TextureCalcBytes(void*,void*);
extern "C" unsigned VolumeVTable[],BaseVTable[];
extern "C" void AbortBoundary(){abort();}
static unsigned arena[4096],initial[4096],images[16][4096];
static unsigned mode,step,count,snapshots,location,profile,mutation,levels,levelCalls,byteCalls,releases,countCalls;
static unsigned* active;
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned address,unsigned offset){return *(unsigned*)(address+offset);}
static void require(bool value){if(!value)abort();}
static unsigned pattern(unsigned n){const unsigned values[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static void snapshot(){if(!mode){require(count<16);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static void event(unsigned kind,unsigned argument){
 word(ptr(0x2000),0)=kind;word(ptr(0x2000),4)=argument;
 word(ptr(0x2000),8)=countCalls;word(ptr(0x2000),12)=levelCalls;
 word(ptr(0x2000),16)=byteCalls;word(ptr(0x2000),20)=releases;
 word(ptr(0x2000),24)=active?active[0]:0;word(ptr(0x2000),28)=active?active[1]:0;snapshot();
}
static unsigned long __stdcall levelCount(void* self){
 require(!active&&!countCalls&&(unsigned)self==word(ptr(0x100),4)&&word(ptr(0x100),8)==0);++countCalls;
 if(mutation&1){word(ptr(0x100),4)=ptr(0x800);word(ptr(0x100),8)=pattern(profile+4);}
 event(1,(unsigned)self);return levels;
}
extern "C" void* __fastcall GetLevel(void* self,void*,void* output,long level){
 require((unsigned)self==ptr(0x100)&&!active&&(unsigned)level==levelCalls&&byteCalls==levelCalls&&releases==levelCalls);++levelCalls;
 active=(unsigned*)output;active[0]=(unsigned)VolumeVTable;active[1]=ptr(levelCalls&1?0xc00:0xe00);
 if(mutation&2){word(ptr(0x100),4)=ptr(levelCalls&1?0x400:0x800);word(ptr(0x100),8)=pattern(profile+levelCalls);word(ptr(0x100),12)=pattern(profile+5);}
 event(2,(unsigned)level);return output;
}
extern "C" unsigned long __fastcall VolumeBytes(void* self,void*){
 require(active==(unsigned*)self&&byteCalls+1==levelCalls&&releases==byteCalls);++byteCalls;
 if(mutation&4){active[0]=pattern(profile+6);active[1]=ptr(0xe00);word(ptr(0x100),8)=pattern(profile+byteCalls+4);}
 event(3,active[1]);return pattern(profile+byteCalls);
}
static unsigned long __stdcall release(void* self){
 require(active&&active[0]==(unsigned)VolumeVTable&&(unsigned)self==active[1]&&releases+1==byteCalls);++releases;
 if(mutation){word(ptr(0x100),8)^=pattern(profile+releases+2);word(ptr(0x100),0)=pattern(profile+7);}
 event(4,(unsigned)self);active=0;return pattern(profile+releases);
}
static void model(){
 word(ptr(0x100),8)=0;
 if(word(ptr(0x100),4)){
  long cached=(long)levelCount((void*)word(ptr(0x100),4));
  for(long level=0;level<cached;++level){
   unsigned volume[2];GetLevel((void*)ptr(0x100),0,volume,level);
   unsigned bytes=VolumeBytes(volume,0);word(ptr(0x100),8)+=bytes;
   volume[0]=(unsigned)VolumeVTable;if(volume[1]){release((void*)volume[1]);volume[1]=0;}volume[0]=(unsigned)BaseVTable;
  }
 }
}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 unsigned targets[4]={(unsigned)VolumeVTable,(unsigned)BaseVTable,(unsigned)&GetLevel,(unsigned)&VolumeBytes};
 for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=fixups[f].target>=0x80000000?targets[fixups[f].target-0x80000000]:(unsigned)(code+fixups[f].target);*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}
 unsigned textureTable[14]={0},volumeTable[3]={0};textureTable[13]=(unsigned)levelCount;volumeTable[2]=(unsigned)release;
 const unsigned levelValues[8]={0,1,2,3,0x80000000,0x80000001,0xfffffffe,0xffffffff};unsigned cases=0;
 for(profile=0;profile<8;++profile)for(unsigned pd=0;pd<3;++pd)for(unsigned level=0;level<8;++level)for(mutation=0;mutation<8;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
  location=place?0x1800:0;levels=levelValues[level];memset(arena,fill?0xa9:0,sizeof(arena));
  word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=pd?ptr(pd==1?0x400:0x800):0;word(ptr(0x100),8)=pattern(profile+1);word(ptr(0x100),12)=pattern(profile+2);
  word(ptr(0x400),0)=word(ptr(0x800),0)=(unsigned)textureTable;word(ptr(0xc00),0)=word(ptr(0xe00),0)=(unsigned)volumeTable;
  memset((void*)ptr(0x2000),0,32);memcpy(initial,arena,sizeof(arena));count=0;
  for(mode=0;mode<3;++mode){
   memcpy(arena,initial,sizeof(arena));step=countCalls=levelCalls=byteCalls=releases=0;active=0;
   if(!mode)model();else(mode==1?((void(__fastcall*)(void*,void*))code):TextureCalcBytes)((void*)ptr(0x100),0);
   require(!active&&countCalls==(pd?1:0)&&levelCalls==((pd&&(long)levels>0)?levels:0)&&byteCalls==levelCalls&&releases==levelCalls);
   snapshot();if(mode)require(step==count);
  }++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_CALC_BYTES_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
