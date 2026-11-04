#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_volume_types.h"
#include "fable_d3d_format_info.h"
#include "volume_texture_create_retail.h"
#include "volume_texture_create_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" bool __fastcall TextureCreate(void*,void*,const C3DExtentsI*,unsigned int,unsigned long,const CPixelFormat*,EVolumePool,bool);
static unsigned arena[4096],initial[4096],images[3][4096];
static unsigned mode,step,count,snapshots,location,profile,mutation,row,dimensions,pixelFormat,levels,usage,pool,status,systemCalls,createCalls;
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned address,unsigned offset){return *(unsigned*)(address+offset);}
static void require(bool value){if(!value)abort();}
static unsigned pattern(unsigned n){const unsigned values[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static void snapshot(){if(!mode){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
extern "C" void* __fastcall SystemGetter(){
 require(!systemCalls&&!createCalls);++systemCalls;unsigned system=ptr(profile&1?0x1600:0x1000);
 if(mutation&1){word(dimensions,0)=pattern(profile+5);word(dimensions,4)=pattern(profile+6);word(dimensions,8)=pattern(profile+7);word(pixelFormat,0)=(row+1)%47;}
 if(mutation&2)word(system,0x60)=ptr(profile&1?0x1200:0x1800);
 word(ptr(0x2000),0)=systemCalls;word(ptr(0x2000),4)=system;snapshot();return (void*)system;
}
extern "C" HRESULT __stdcall CreateVolume(IDirect3DDevice9* device,unsigned int width,unsigned int height,unsigned int depth,unsigned int mipLevels,unsigned long flags,D3DFORMAT format,D3DPOOL memoryPool,IDirect3DVolumeTexture9** output){
 unsigned system=ptr(profile&1?0x1600:0x1000);unsigned display=word(system,0x60);
 require(systemCalls==1&&!createCalls&&(unsigned)device==word(display,0x58)&&width==word(dimensions,0)&&height==word(dimensions,4)&&depth==word(dimensions,8)&&mipLevels==levels&&flags==usage&&(unsigned)format==(unsigned)GD3DFormats[word(pixelFormat,0)].D3DFormat&&(unsigned)memoryPool==pool&&(unsigned)output==ptr(0x104));++createCalls;
 word(ptr(0x2000),8)=createCalls;word(ptr(0x2000),12)=(unsigned)device;word(ptr(0x2000),16)=width;word(ptr(0x2000),20)=height;word(ptr(0x2000),24)=depth;word(ptr(0x2000),28)=mipLevels;word(ptr(0x2000),32)=flags;word(ptr(0x2000),36)=(unsigned)format;word(ptr(0x2000),40)=(unsigned)memoryPool;word(ptr(0x2000),44)=(unsigned)output;
 *output=(IDirect3DVolumeTexture9*)pattern(profile+row+3);
 if(mutation&1){word(ptr(0x100),0)=pattern(profile+6);word(ptr(0x100),8)=pattern(profile+7);word(ptr(0x100),12)=pattern(profile+5);}
 if(mutation&2){word(dimensions,0)^=0xffffffff;word(dimensions,4)=pattern(profile+2);word(dimensions,8)=pattern(profile+4);word(pixelFormat,0)=(row+3)%47;word(system,0x60)=ptr(0x1200);word(display,0x58)=pattern(profile+3);}
 snapshot();return (HRESULT)status;
}
static bool model(){
 unsigned system=(unsigned)SystemGetter();IDirect3DDevice9* device=(IDirect3DDevice9*)word(word(system,0x60),0x58);D3DFORMAT format=GD3DFormats[word(pixelFormat,0)].D3DFormat;
 HRESULT result=CreateVolume(device,word(dimensions,0),word(dimensions,4),word(dimensions,8),levels,usage,format,(D3DPOOL)pool,(IDirect3DVolumeTexture9**)ptr(0x104));return result>=0;
}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 unsigned targets[3]={(unsigned)&SystemGetter,(unsigned)GD3DFormats,(unsigned)&CreateVolume};
 for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=targets[fixups[f].target];*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}
 const unsigned profiles[2]={0,7},pools[4]={0,1,2,0xffffffff},statuses[8]={0,1,2,0x7fffffff,0x80000000,0x80004005,0x8007000e,0xffffffff},dimensionOffsets[3]={0x200,0x100,0x104},formatOffsets[3]={0x300,0x100,0x104};unsigned cases=0;
 for(row=0;row<47;++row)for(unsigned p=0;p<2;++p)for(unsigned memoryPool=0;memoryPool<4;++memoryPool)for(unsigned result=0;result<4;++result)for(unsigned d=0;d<3;++d)for(unsigned f=0;f<3;++f)for(unsigned last=0;last<2;++last)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
  profile=profiles[p];location=place?0x1800:0;dimensions=ptr(dimensionOffsets[d]);pixelFormat=ptr(formatOffsets[f]);levels=pattern(profile+row);usage=pattern(profile+row+3);pool=pools[memoryPool];status=statuses[result+(p?4:0)];memset(arena,fill?0xa9:0,sizeof(arena));
  word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=pattern(profile+1);word(ptr(0x100),8)=pattern(profile+2);word(ptr(0x100),12)=pattern(profile+3);word(dimensions,0)=pattern(profile+row+4);word(dimensions,4)=pattern(profile+row+5);word(dimensions,8)=pattern(profile+row+6);word(pixelFormat,0)=row;
  word(ptr(0x1000),0x60)=ptr(0x1200);word(ptr(0x1200),0x58)=ptr(0x1400);word(ptr(0x1600),0x60)=ptr(0x1800);word(ptr(0x1800),0x58)=ptr(0x1a00);memset((void*)ptr(0x2000),0,52);memcpy(initial,arena,sizeof(arena));count=0;
  for(mode=0;mode<3;++mode){
   memcpy(arena,initial,sizeof(arena));step=systemCalls=createCalls=0;
   bool returned=!mode?model():(mode==1?((bool(__fastcall*)(void*,void*,const C3DExtentsI*,unsigned int,unsigned long,const CPixelFormat*,EVolumePool,bool))code):TextureCreate)((void*)ptr(0x100),0,(const C3DExtentsI*)dimensions,levels,usage,(const CPixelFormat*)pixelFormat,(EVolumePool)pool,last!=0);
   require(returned==((long)status>=0)&&systemCalls==1&&createCalls==1);word(ptr(0x2000),48)=returned;snapshot();if(mode)require(step==count);
  }++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_CREATE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
