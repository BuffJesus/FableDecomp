#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_volume_types.h"
#include "volume_texture_lock_retail.h"
#include "volume_texture_lock_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void* __fastcall TextureLock(void*,void*,void*,unsigned long,unsigned long);
extern "C" void __fastcall ExecuteWait(void*,void*,void*,float);
static unsigned arena[4096],initial[4096],images[5][4096];
static unsigned mode,step,count,snapshots,location,profile,mutation,level,flags,status,output,descriptions,systems,waits,locks;
static D3DVOLUME_DESC* liveDescription;
static unsigned char* code;
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned address,unsigned offset){return *(unsigned*)(address+offset);}
static unsigned pattern(unsigned n){const unsigned values[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static void require(bool value){if(!value)abort();}
static void snapshot(){if(!mode){require(count<5);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static void event(unsigned kind,unsigned argument){
 word(ptr(0x2000),0)=kind;word(ptr(0x2000),4)=argument;word(ptr(0x2000),8)=descriptions;word(ptr(0x2000),12)=systems;word(ptr(0x2000),16)=waits;word(ptr(0x2000),20)=locks;
 word(ptr(0x2000),24)=liveDescription?liveDescription->Width:0;word(ptr(0x2000),28)=liveDescription?liveDescription->Height:0;word(ptr(0x2000),32)=liveDescription?liveDescription->Depth:0;snapshot();
}
static long __stdcall description(void* self,unsigned index,D3DVOLUME_DESC* result){
 require(!descriptions&&!systems&&!waits&&!locks&&!liveDescription&&(unsigned)self==word(ptr(0x100),4)&&index==level);++descriptions;liveDescription=result;
 memset(result,0xa9,sizeof(*result));result->Width=pattern(profile);result->Height=pattern(profile+1);result->Depth=pattern(profile+2);
 event(1,(unsigned)self);return (long)pattern(profile+6);
}
extern "C" void* __fastcall SystemGetter(){
 require(descriptions==1&&!systems&&!waits&&!locks&&liveDescription);++systems;
 if(mutation&1){liveDescription->Width=pattern(profile+4);liveDescription->Height=pattern(profile+5);liveDescription->Depth=pattern(profile+6);}
 if(mutation&2)word(ptr(0x100),4)=ptr(word(ptr(0x100),4)==ptr(0x400)?0x800:0x400);
 if(mutation&4){word(ptr(0x100),0)=pattern(profile+7);word(ptr(0x100),8)=pattern(profile+5);word(ptr(0x100),12)=pattern(profile+3);}
 unsigned system=ptr(profile&1?0x1600:0x1000);event(2,system);return (void*)system;
}
extern "C" void __fastcall ObserveWait(void* self,void*,void* resource,float timeout){
 unsigned bits;memcpy(&bits,&timeout,4);require(descriptions==1&&systems==1&&!waits&&!locks&&liveDescription&&(unsigned)self==ptr(profile&1?0x1a00:0x1400)&&(unsigned)resource==word(ptr(0x100),4)&&bits==0x3f800000);++waits;
 word(ptr(0x2000),36)=(unsigned)self;word(ptr(0x2000),40)=bits;event(3,(unsigned)resource);
 if(mode)(mode==1?((void(__fastcall*)(void*,void*,void*,float))(code+192)):ExecuteWait)(self,0,resource,timeout);
}
static long __stdcall lockBox(void* self,unsigned index,D3DLOCKED_BOX* locked,const D3DBOX* box,unsigned lockFlags){
 require(descriptions==1&&systems==1&&waits==1&&!locks&&liveDescription&&(unsigned)self==word(ptr(0x100),4)&&index==level&&lockFlags==flags&&box->Left==0&&box->Top==0&&box->Front==0&&box->Right==pattern(profile)&&box->Bottom==pattern(profile+1)&&box->Back==pattern(profile+2));++locks;
 locked->RowPitch=(int)pattern(profile+1);locked->SlicePitch=(int)pattern(profile+4);locked->pBits=(void*)pattern(profile+7);
 if(mutation&1){liveDescription->Width=pattern(profile+5);liveDescription->Height=pattern(profile+6);liveDescription->Depth=pattern(profile+7);}
 if(mutation&2){locked->RowPitch=(int)pattern(profile+2);locked->SlicePitch=(int)pattern(profile+5);locked->pBits=(void*)pattern(profile+6);}
 if(mutation&4){word(ptr(0x100),4)=0;word(ptr(0x100),0)=pattern(profile+3);word((unsigned)self,0)=pattern(profile+4);}
 word(ptr(0x2000),44)=index;word(ptr(0x2000),48)=lockFlags;word(ptr(0x2000),52)=box->Right;word(ptr(0x2000),56)=box->Bottom;word(ptr(0x2000),60)=box->Back;word(ptr(0x2000),64)=(unsigned)locked->RowPitch;word(ptr(0x2000),68)=(unsigned)locked->SlicePitch;word(ptr(0x2000),72)=(unsigned)locked->pBits;
 event(4,(unsigned)self);liveDescription=0;return (long)status;
}
static unsigned model(){
 D3DVOLUME_DESC desc;description((void*)word(ptr(0x100),4),level,&desc);unsigned width=desc.Width,height=desc.Height;D3DBOX box={0,0,width,height,0,desc.Depth};
 unsigned system=(unsigned)SystemGetter();ObserveWait((void*)word(word(system,0x60),8),0,(void*)word(ptr(0x100),4),1.0f);
 D3DLOCKED_BOX locked;long result=lockBox((void*)word(ptr(0x100),4),level,&locked,&box,flags);
 if(result>=0){word(output,0)=width;word(output,4)=height;word(output,8)=desc.Depth;word(output,12)=(unsigned)locked.RowPitch;word(output,16)=(unsigned)locked.SlicePitch;word(output,20)=(unsigned)locked.pBits;}
 return output;
}
int main(){
 code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[2]={(unsigned)&SystemGetter,(unsigned)&ObserveWait};
 for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=targets[fixups[f].target];*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}
 unsigned table[20]={0};table[17]=(unsigned)description;table[19]=(unsigned)lockBox;const unsigned statuses[8]={0,1,2,0x7fffffff,0x80000000,0x80004005,0xfffffffe,0xffffffff},outputs[5]={0x600,0x100,0x104,0x400,0x800};unsigned cases=0;
 for(profile=0;profile<8;++profile)for(unsigned result=0;result<8;++result)for(unsigned out=0;out<5;++out)for(unsigned pd=0;pd<2;++pd)for(mutation=0;mutation<8;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
  location=place?0x1800:0;level=pattern(profile);flags=pattern(profile+3);status=statuses[result];output=ptr(outputs[out]);memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=ptr(pd?0x800:0x400);word(ptr(0x100),8)=pattern(profile+4);word(ptr(0x100),12)=pattern(profile+5);word(ptr(0x400),0)=word(ptr(0x800),0)=(unsigned)table;
  word(ptr(0x1000),0x60)=ptr(0x1200);word(ptr(0x1200),8)=ptr(0x1400);word(ptr(0x1600),0x60)=ptr(0x1800);word(ptr(0x1800),8)=ptr(0x1a00);memset((void*)ptr(0x2000),0,80);memcpy(initial,arena,sizeof(arena));count=0;
  for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=descriptions=systems=waits=locks=0;liveDescription=0;unsigned returned=!mode?model():(unsigned)(mode==1?((void*(__fastcall*)(void*,void*,void*,unsigned long,unsigned long))code):TextureLock)((void*)ptr(0x100),0,(void*)output,level,flags);require(returned==output&&!liveDescription&&descriptions==1&&systems==1&&waits==1&&locks==1);word(ptr(0x2000),76)=returned;snapshot();if(mode)require(step==count);}++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_LOCK_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
