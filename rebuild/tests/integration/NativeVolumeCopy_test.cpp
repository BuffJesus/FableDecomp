#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "volume_copy_retail.h"
#include "volume_copy_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void* __fastcall VolumeCopy(void*,void*,const void*);
extern "C" unsigned VolumeVTable[],BaseVTable[];
extern "C" void AbortBoundary(){abort();}
static unsigned arena[4096],initial[4096],images[2][4096],mode,step,count,snapshots,location,profile,mutation,calls,pointee,source;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void snapshot(){if(!mode){require(count<2);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static long __stdcall addRef(void* volume){require(!calls&&(unsigned)volume==pointee&&word(ptr(0x100),0)==(unsigned)VolumeVTable&&word(ptr(0x100),4)==pointee);++calls;word(ptr(0x2000),0)=calls;word(ptr(0x2000),4)=(unsigned)volume;if(mutation==1)word(source,4)=ptr(0xa00);if(mutation==2){word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=0;}if(mutation==3){word(source,4)=0;word(pointee,0)=pattern(profile+1);word(pointee,4)^=0xffffffff;}snapshot();return (long)pattern(profile);}
static unsigned model(){word(ptr(0x100),0)=(unsigned)BaseVTable;word(ptr(0x100),0)=(unsigned)VolumeVTable;unsigned p=word(source,4);word(ptr(0x100),4)=p;if(p)addRef((void*)p);return ptr(0x100);}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[2]={(unsigned)VolumeVTable,(unsigned)BaseVTable};for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=fixups[f].target>=0x80000000?targets[fixups[f].target-0x80000000]:(unsigned)(code+fixups[f].target);*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}unsigned table[3]={0};table[1]=(unsigned)addRef;unsigned cases=0;
for(profile=0;profile<8;++profile)for(mutation=0;mutation<4;++mutation)for(unsigned object=0;object<3;++object)for(unsigned self=0;self<2;++self)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;pointee=object?ptr(object==1?0x400:0x800):0;source=ptr(self?0x100:0x300);memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=pattern(profile+1);word(source,0)=pattern(profile+2);word(source,4)=pointee;if(pointee){word(pointee,0)=(unsigned)table;word(pointee,4)=pattern(profile+3);}memset((void*)ptr(0x2000),0,28);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=calls=0;unsigned result=!mode?model():(unsigned)(mode==1?((void*(__fastcall*)(void*,void*,const void*))code):VolumeCopy)((void*)ptr(0x100),0,(const void*)source);word(ptr(0x2000),24)=result;snapshot();if(mode)require(step==count&&calls==(pointee?1:0)&&result==ptr(0x100));}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_COPY_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
