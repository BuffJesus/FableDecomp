#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counted_volume_texture_reset_retail.h"
#include "counted_volume_texture_reset_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall CountedVolumeTextureReset(void*,void*,void*);
extern "C" void __fastcall VolumeTextureDeleteData(void*);
extern "C" void AbortBoundary(){abort();}
static unsigned arena[4096],initial[4096],images[4][4096],mode,step,count,snapshots,location,profile,mutation;
static unsigned owner,newData,newBlock,oldInfo,oldCount,dropCalls,deleteCalls,allocCalls,lastDelete;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void snapshot(){if(!mode){require(count<4);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static void event(unsigned kind,unsigned argument){word(ptr(0x2000),0)=kind;word(ptr(0x2000),4)=argument;word(ptr(0x2000),8)=dropCalls;word(ptr(0x2000),12)=deleteCalls;word(ptr(0x2000),16)=allocCalls;snapshot();}
static void __fastcall drop(void* data){require(!dropCalls&&!deleteCalls&&!allocCalls&&oldInfo&&oldCount==1&&(unsigned)data==word(oldInfo,8));++dropCalls;if(mutation&1){word(owner,4)=oldInfo==ptr(0x400)?ptr(0x600):ptr(0x400);word(owner,0)=pattern(profile+3);word(oldInfo,0)=pattern(profile+4);}event(1,(unsigned)data);}
extern "C" void Delete(void* value){require(dropCalls==1&&!deleteCalls&&!allocCalls&&(unsigned)value==word(owner,4));++deleteCalls;lastDelete=(unsigned)value;if(mutation&2){word(owner,4)=ptr(0x600);word(owner,0)=pattern(profile+5);word((unsigned)value,0)^=pattern(profile+6);}event(2,(unsigned)value);}
void operator delete(void* value){Delete(value);}
extern "C" void* Allocate(unsigned size){require(size==12&&newData&&!allocCalls&&!word(owner,4)&&word(owner,0)==newData);++allocCalls;if(mutation&2){word(owner,0)=ptr(0xa00);word(owner,4)=ptr(0x400);if(newBlock){word(newBlock,0)=pattern(profile+1);word(newBlock,4)=pattern(profile+2);word(newBlock,8)=pattern(profile+3);}}event(3,size);return (void*)newBlock;}
void* operator new(unsigned size){return Allocate(size);}
static void model(){if(word(owner,4)){--word(word(owner,4),0);if(!word(word(owner,4),0)){unsigned info=word(owner,4);drop((void*)word(info,8));Delete((void*)word(owner,4));}}word(owner,4)=0;word(owner,0)=newData;if(newData){unsigned block=(unsigned)Allocate(12);if(block){word(block,0)=1;word(block,4)=(unsigned)VolumeTextureDeleteData;word(block,8)=word(owner,0);}word(owner,4)=block;}}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[3]={(unsigned)Allocate,(unsigned)Delete,(unsigned)VolumeTextureDeleteData};for(unsigned f=0;f<sizeof(fixups)/sizeof(*fixups);++f){require(fixups[f].target<3);unsigned target=targets[fixups[f].target];*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}unsigned cases=0;
for(profile=0;profile<8;++profile)for(unsigned oi=0;oi<3;++oi)for(unsigned rc=0;rc<8;++rc)for(unsigned nd=0;nd<4;++nd)for(unsigned nb=0;nb<3;++nb)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;owner=ptr(0x100);oldInfo=oi?ptr(oi==1?0x400:0x600):0;oldCount=pattern(rc);unsigned data[4]={0,ptr(0xa00),ptr(0x800),owner};newData=data[nd];newBlock=nb?ptr(nb==1?0xe00:0x1000):0;memset(arena,fill?0xa9:0,sizeof(arena));word(owner,0)=ptr(0x800);word(owner,4)=oldInfo;for(unsigned i=0;i<2;++i){unsigned b=ptr(i?0x600:0x400);word(b,0)=oldCount;word(b,4)=(unsigned)drop;word(b,8)=ptr(i?0xc00:0x800);}memset((void*)ptr(0x2000),0,24);memcpy(initial,arena,sizeof(arena));count=0;
for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=dropCalls=deleteCalls=allocCalls=lastDelete=0;if(!mode)model();else(mode==1?((void(__fastcall*)(void*,void*,void*))code):CountedVolumeTextureReset)((void*)owner,0,(void*)newData);require(dropCalls==(oldInfo&&oldCount==1?1:0)&&deleteCalls==dropCalls&&allocCalls==(newData?1:0));if(newData&&newBlock){require(word(newBlock,0)==1&&word(newBlock,4)==(unsigned)VolumeTextureDeleteData&&word(newBlock,8)==word(owner,0)&&word(owner,4)==newBlock);}else require(!word(owner,4));word(ptr(0x2000),20)=lastDelete;snapshot();if(mode)require(step==count);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_COUNTED_VOLUME_TEXTURE_RESET_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}

