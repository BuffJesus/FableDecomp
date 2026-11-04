#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_volume_types.h"
#include "fable_d3dx9_texture.h"
#include "volume_copy_from_volume_retail.h"
#include "volume_copy_from_volume_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall VolumeCopyFromVolume(void*,void*,const void*,const void*,const void*,unsigned long);
static unsigned arena[4096],initial[4096],images[2][4096],mode,step,count,snapshots,location,profile,mutation,calls;
static unsigned owner,source,destinationBox,sourceBox,expectedDestination[6],expectedSource[6],destinationPD,sourcePD;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void snapshot(){if(!mode){require(count<2);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
extern "C" HRESULT __stdcall LoadVolume(IDirect3DVolume9* dst,const PALETTEENTRY* dp,const D3DBOX* db,IDirect3DVolume9* src,const PALETTEENTRY* sp,const D3DBOX* sb,unsigned long filter,D3DCOLOR key){
 require(!calls&&(unsigned)dst==destinationPD&&(unsigned)src==sourcePD&&!dp&&!sp&&!key&&filter==pattern(profile+3));++calls;
 require((db!=0)==(destinationBox!=0)&&(sb!=0)==(sourceBox!=0));
 if(db)require((unsigned)db!=destinationBox&&!memcmp(db,expectedDestination,24));
 if(sb)require((unsigned)sb!=sourceBox&&!memcmp(sb,expectedSource,24));
 unsigned log=ptr(0x2000);word(log,0)=calls;word(log,4)=(unsigned)dst;word(log,8)=(unsigned)src;word(log,12)=filter;word(log,16)=pattern(profile+5);
 if(db)memcpy((void*)(log+24),db,24);if(sb)memcpy((void*)(log+48),sb,24);
 if(mutation&1){if(destinationBox)for(unsigned i=0;i<6;++i)word(destinationBox,i*4)=pattern(profile+i+4);if(sourceBox)for(unsigned i=0;i<6;++i)word(sourceBox,i*4)=pattern(profile+i+5);word(owner,4)=pattern(profile+1);word(source,4)=pattern(profile+2);}
 if(db)require(!memcmp(db,expectedDestination,24));if(sb)require(!memcmp(sb,expectedSource,24));
 if(mutation&2){if(db)((D3DBOX*)db)->Back^=0xffffffff;if(sb)((D3DBOX*)sb)->Front^=0xffffffff;word(owner,0)^=0x12345678;word(source,0)^=0x87654321;}
 snapshot();return (HRESULT)pattern(profile+5);
}
static void convert(unsigned input,unsigned* output){for(unsigned i=0;i<6;++i)output[i]=word(input,i*4);}
static void model(){D3DBOX db,sb;if(destinationBox)convert(destinationBox,(unsigned*)&db);if(sourceBox)convert(sourceBox,(unsigned*)&sb);LoadVolume((IDirect3DVolume9*)word(owner,4),0,destinationBox?&db:0,(IDirect3DVolume9*)word(source,4),0,sourceBox?&sb:0,pattern(profile+3),0);}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));for(unsigned f=0;f<sizeof(fixups)/sizeof(*fixups);++f){require(fixups[f].kind==20);*(unsigned*)(code+fixups[f].site)=(unsigned)LoadVolume-(unsigned)(code+fixups[f].site+4);}unsigned cases=0;
for(profile=0;profile<8;++profile)for(unsigned self=0;self<2;++self)for(unsigned dc=0;dc<4;++dc)for(unsigned sc=0;sc<4;++sc)for(unsigned dpc=0;dpc<3;++dpc)for(unsigned spc=0;spc<3;++spc)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;owner=ptr(0x100);source=self?owner:ptr(0x300);unsigned ds[4]={0,ptr(0x600),owner,ptr(0x800)},ss[4]={0,ptr(0x600),ptr(0x300),ptr(0x804)};destinationBox=ds[dc];sourceBox=ss[sc];memset(arena,fill?0xa9:0,sizeof(arena));word(owner,4)=dpc==0?0:dpc==1?1:0xffffffff;if(!self)word(source,4)=spc==0?0:spc==1?1:0xffffffff;
 if(destinationBox)for(unsigned i=0;i<6;++i)word(destinationBox,i*4)=pattern(profile+i);if(sourceBox)for(unsigned i=0;i<6;++i)word(sourceBox,i*4)=pattern(profile+i+2);
 memset((void*)ptr(0x2000),0,80);destinationPD=word(owner,4);sourcePD=word(source,4);if(destinationBox)convert(destinationBox,expectedDestination);if(sourceBox)convert(sourceBox,expectedSource);memcpy(initial,arena,sizeof(arena));count=0;
 for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=calls=0;if(!mode)model();else (mode==1?((void(__fastcall*)(void*,void*,const void*,const void*,const void*,unsigned long))code):VolumeCopyFromVolume)((void*)owner,0,(void*)destinationBox,(void*)source,(void*)sourceBox,pattern(profile+3));word(ptr(0x2000),20)=calls;snapshot();if(mode)require(step==count&&calls==1);}++cases;}
 VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_COPY_FROM_VOLUME_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
