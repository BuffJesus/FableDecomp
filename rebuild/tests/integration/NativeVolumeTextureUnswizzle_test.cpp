#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "volume_texture_unswizzle_retail.h"
#include "volume_texture_unswizzle_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall TextureUnswizzle(void*,void*,unsigned int,void*);
extern "C" void __fastcall VolumeUnswizzle(void*,void*,void*);
extern "C" unsigned VolumeVTable[],BaseVTable[];
extern "C" void AbortBoundary(){abort();}
static unsigned arena[4096],initial[4096],images[3][4096],mode,step,count,snapshots,location,profile,mutation,level,pd,getCalls,releases,box;
static unsigned* active;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void snapshot(){if(!mode){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static void event(unsigned kind){word(ptr(0x2000),0)=kind;word(ptr(0x2000),4)=getCalls;word(ptr(0x2000),8)=releases;word(ptr(0x2000),12)=active[0];word(ptr(0x2000),16)=active[1];snapshot();}
extern "C" void* __fastcall GetLevel(void* self,void*,void* output,long requested){require((unsigned)self==ptr(0x100)&&!getCalls&&!active&&(unsigned)requested==level);++getCalls;active=(unsigned*)output;active[0]=pattern(profile);active[1]=pd?ptr(pd==1?0xc00:0xe00):0;if(mutation&1){word(ptr(0x100),4)=pattern(profile+1);word(box,0)^=pattern(profile+2);word(box,20)=pattern(profile+3);}event(1);return output;}
static unsigned long __stdcall release(void* self){require(active&&active[0]==(unsigned)VolumeVTable&&(unsigned)self==active[1]&&!releases);++releases;if(mutation&2){word(ptr(0x100),8)^=pattern(profile+4);word(box,12)^=pattern(profile+5);}event(2);active=0;return pattern(profile+6);}
static void model(){unsigned volume[2];GetLevel((void*)ptr(0x100),0,volume,(long)level);volume[0]=(unsigned)VolumeVTable;if(volume[1]){release((void*)volume[1]);volume[1]=0;}volume[0]=(unsigned)BaseVTable;active=0;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[3]={(unsigned)VolumeVTable,(unsigned)BaseVTable,(unsigned)GetLevel};for(unsigned f=0;f<sizeof(fixups)/sizeof(*fixups);++f){unsigned target=fixups[f].target>=0x80000000?targets[fixups[f].target-0x80000000]:(unsigned)(code+fixups[f].target);*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}unsigned table[3]={0};table[2]=(unsigned)release;unsigned cases=0;
for(profile=0;profile<8;++profile)for(unsigned lv=0;lv<8;++lv)for(pd=0;pd<3;++pd)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;level=pattern(lv);box=ptr(profile&1?0x100:0x600);memset(arena,fill?0xa9:0,sizeof(arena));for(unsigned i=0;i<6;++i)word(box,i*4)=pattern(profile+i);word(ptr(0xc00),0)=word(ptr(0xe00),0)=(unsigned)table;memset((void*)ptr(0x2000),0,24);memcpy(initial,arena,sizeof(arena));count=0;
for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=getCalls=releases=0;active=0;if(!mode)model();else(mode==1?((void(__fastcall*)(void*,void*,unsigned int,void*))code):TextureUnswizzle)((void*)ptr(0x100),0,level,(void*)box);active=0;require(getCalls==1&&releases==(pd?1:0));if(mode==1)((void(__fastcall*)(void*,void*,void*))(code+80))((void*)pattern(profile),0,(void*)box);else if(mode==2)VolumeUnswizzle((void*)pattern(profile),0,(void*)box);snapshot();if(mode)require(step==count);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_UNSWIZZLE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
