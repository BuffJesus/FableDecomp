#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_surface_types.h"
#include "managed_texture_data_create_retail.h"
#include "managed_texture_data_create_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" bool __fastcall ManagedCreate(void*,void*,unsigned long,unsigned long,unsigned,const CPixelFormat*);
static unsigned arena[4096],initial[4096],expected[3][4096],captures[3][4096];
static unsigned ownerOffset,width,height,profile,mutation,captured;static bool success;
static void require(bool value){if(!value)abort();}
static unsigned pattern(unsigned n){const unsigned values[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static CPixelFormat* format(){return (CPixelFormat*)(arena+0x600/4);}
static void capture(){require(captured<3);memcpy(captures[captured++],arena,sizeof(arena));}
extern "C" void* __fastcall DimensionsBoundary(void* output,void* input,const CPixelFormat* pixelFormat){
 require(pixelFormat==format());unsigned* in=(unsigned*)input;require(in[0]==width&&in[1]==height);arena[0x2000/4]++;arena[0x2000/4+1]=in[0];arena[0x2000/4+2]=in[1];capture();
 unsigned* owner=arena+ownerOffset;if(mutation&1){owner[8]=pattern(profile+5);owner[7]=0xdeadcafe;*(unsigned*)format()^=0xdeadbeef;in[0]^=0xffffffff;in[1]^=0x80000000;}
 ((unsigned*)output)[0]=pattern(profile+2);((unsigned*)output)[1]=pattern(profile+4);return output;
}
bool CTexture::Initialise(const C2DExtentsI& dimensions,long levels,const CPixelFormat& pixelFormat,unsigned long flags,ESurfacePool pool,bool dynamic,bool automaticMipmaps){
 unsigned* owner=arena+ownerOffset;require((unsigned)this==owner[8]&&&pixelFormat==format());require(levels==-1&&flags==0&&pool==SURFACE_POOL_MANAGED&&!dynamic&&!automaticMipmaps);require((unsigned)dimensions.X==pattern(profile+2)&&(unsigned)dimensions.Y==pattern(profile+4));require(((unsigned short*)owner)[14]==(unsigned short)dimensions.X&&((unsigned short*)owner)[15]==(unsigned short)dimensions.Y);
 arena[0x2000/4+3]++;arena[0x2000/4+4]=(unsigned)this;arena[0x2000/4+5]=(unsigned)dimensions.X;arena[0x2000/4+6]=(unsigned)dimensions.Y;capture();
 if(mutation&2){memset((unsigned char*)owner+28,0xcc,48);C2DExtentsI& live=const_cast<C2DExtentsI&>(dimensions);live.X=0x12345678;live.Y=0x87654321;}
 return success;
}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));bool(CTexture::*method)(const C2DExtentsI&,long,const CPixelFormat&,unsigned long,ESurfacePool,bool,bool)=&CTexture::Initialise;unsigned initAddress=0;require(sizeof(method)==4);memcpy(&initAddress,&method,4);unsigned targets[2]={(unsigned)DimensionsBoundary,initAddress};require(sizeof(fixups)/sizeof(*fixups)==2);for(unsigned f=0;f<2;++f){require(fixups[f].kind==20&&fixups[f].target<2);*(unsigned*)(code+fixups[f].site)=targets[fixups[f].target]-(unsigned)(code+fixups[f].site+4);}unsigned cases=0,snapshots=0;
for(unsigned wi=0;wi<8;++wi)for(unsigned hi=0;hi<4;++hi)for(unsigned li=0;li<4;++li)for(profile=0;profile<4;++profile)for(mutation=0;mutation<4;++mutation)for(unsigned result=0;result<2;++result)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
 ownerOffset=(place?0x1800:0)/4+0x100/4;width=pattern(wi);height=pattern(hi*2);success=result!=0;unsigned* owner=arena+ownerOffset;memset(arena,fill?0xa9:0,sizeof(arena));for(unsigned i=0;i<19;++i)owner[i]=pattern(profile+i);memset(arena+0x2000/4,0,32);memcpy(initial,arena,sizeof(arena));
 for(unsigned mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));captured=0;bool returned;if(!mode){unsigned input[2]={width,height},output[2];DimensionsBoundary(output,input,format());((unsigned short*)owner)[14]=(unsigned short)output[0];((unsigned short*)owner)[15]=(unsigned short)output[1];C2DExtentsI dimensions(output[0],output[1]);returned=((CTexture*)owner[8])->Initialise(dimensions,-1,*format(),0,SURFACE_POOL_MANAGED,false,false);((unsigned char*)owner)[72]=returned;}else returned=(mode==1?((bool(__fastcall*)(void*,void*,unsigned long,unsigned long,unsigned,const CPixelFormat*))code):ManagedCreate)(owner,0,width,height,pattern(li*2),format());require(returned==success);capture();require(captured==3);if(!mode)memcpy(expected,captures,sizeof(expected));else for(unsigned i=0;i<3;++i){require(!memcmp(expected[i],captures[i],sizeof(arena)));++snapshots;}}++cases;
}VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_DATA_CREATE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
