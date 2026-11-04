#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_graphic_info.h"
#include "frame_data_sizeof_retail.h"
#include "frame_data_sizeof_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned long __fastcall FrameDataSizeof(void*,void*);
static unsigned arena[4096],initial[4096],images[3][4096],mode,step,count,snapshots,location,mutation,frameBytes,managedBytes;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static unsigned event(unsigned object,unsigned kind){word(ptr(0x1000),0)=object;word(ptr(0x1000),4)=kind;if(!kind){if(mutation&1)word(ptr(0x100),44)=0;if(mutation&2)word(ptr(0x100),44)=ptr(0x700);if(mutation&4)word(ptr(0x100),28)=ptr(0x500);}else{word(ptr(0x100),28)=0;word(ptr(0x100),44)=ptr(0x600);}snapshot();return kind?managedBytes:frameBytes;}
unsigned long CGraphicFrameData::Sizeof()const{return event((unsigned)this,0);}
unsigned long CManagedTexture::Sizeof()const{return event((unsigned)this,1);}
static unsigned model(){unsigned frame=word(ptr(0x100),28),size=72;if(frame)size+=event(frame,0);if(word(ptr(0x100),44))size+=event(ptr(0x128),1);return size;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned long(CGraphicFrameData::*f)()const=&CGraphicFrameData::Sizeof;unsigned long(CManagedTexture::*m)()const=&CManagedTexture::Sizeof;unsigned targets[2];memcpy(targets,&f,4);memcpy(targets+1,&m,4);for(unsigned i=0;i<2;++i)*(unsigned*)(code+sites[i])=targets[i]-(unsigned)(code+sites[i]+4);const unsigned raw[6]={0,1,4,0x7fffffff,0x80000000,0xffffffff};unsigned cases=0;
for(unsigned frame=0;frame<2;++frame)for(unsigned managed=0;managed<2;++managed)for(unsigned fb=0;fb<6;++fb)for(unsigned mb=0;mb<6;++mb)for(mutation=0;mutation<8;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;frameBytes=raw[fb];managedBytes=raw[mb];memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),28)=frame?ptr(0x400):0;word(ptr(0x100),44)=managed?ptr(0x600):0;memcpy(initial,arena,sizeof(arena));unsigned expected=0;count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=0;unsigned result=!mode?model():(mode==1?((unsigned long(__fastcall*)(void*,void*))code):FrameDataSizeof)((void*)ptr(0x100),0);snapshot();if(!mode)expected=result;else require(result==expected&&step==count);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_FRAME_DATA_SIZEOF_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
