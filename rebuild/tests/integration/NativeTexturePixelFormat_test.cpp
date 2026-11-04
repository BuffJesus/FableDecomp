#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_surface_types.h"
#include "fable_d3d_format_info.h"
#include "texture_pixel_format_retail.h"
#include "texture_pixel_format_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" CPixelFormat* __fastcall TexturePixelFormat(void*,void*,CPixelFormat*);
static unsigned arena[4096],initial[4096],images[3][4096],mode,step,count,snapshots,location,formatProfile,profile,mutation,calls,output;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void snapshot(){if(!mode){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static long __stdcall levelDesc(void* texture,unsigned long level,D3DSURFACE_DESC* description){require(!calls&&level==0&&(unsigned)texture==ptr(0x400));++calls;memset(description,0xa9,sizeof(*description));description->Format=(D3DFORMAT)(formatProfile<46?retailFormats[formatProfile*9]:(formatProfile==46?0xdeadbeef:0));description->Width=pattern(profile+mutation);description->Height=pattern(profile+3);word(ptr(0x2000),0)=calls;word(ptr(0x2000),4)=(unsigned)texture;word(ptr(0x2000),8)=level;word(ptr(0x2000),12)=description->Width;word(ptr(0x2000),16)=description->Height;word(ptr(0x2000),20)=(unsigned)description->Format;if(mutation){word(ptr(0x100),0)=ptr(0x800);word(ptr(0x100),4)^=0xffffffff;word(output,0)=pattern(profile+2);}snapshot();return mutation==3?(long)0x80004005:0;}
static unsigned model(){D3DSURFACE_DESC description;levelDesc((void*)word(ptr(0x100),0),0,&description);unsigned index=0xffffffff;for(unsigned row=0;row<46;++row)if(retailFormats[row*9]==(unsigned)description.Format){index=row;break;}word(output,0)=index;return output;}
int main(){require(!memcmp(retailFormats,GD3DFormats,sizeof(retailFormats)));unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[2]={(unsigned)GD3DFormats,(unsigned)GD3DFormats+8};for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=fixups[f].target>=0x80000000?targets[fixups[f].target-0x80000000]:(unsigned)(code+fixups[f].target);*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}unsigned table[18]={0};table[17]=(unsigned)levelDesc;const unsigned outputs[4]={0x600,0x100,0x104,0x400};unsigned cases=0;
for(formatProfile=0;formatProfile<48;++formatProfile)for(profile=0;profile<8;++profile)for(unsigned out=0;out<4;++out)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;output=ptr(outputs[out]);memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),0)=ptr(0x400);word(ptr(0x100),4)=pattern(profile);word(ptr(0x400),0)=word(ptr(0x800),0)=(unsigned)table;memset((void*)ptr(0x2000),0,28);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=calls=0;unsigned result=!mode?model():(unsigned)(mode==1?((CPixelFormat*(__fastcall*)(void*,void*,CPixelFormat*))code):TexturePixelFormat)((void*)ptr(0x100),0,(CPixelFormat*)output);word(ptr(0x2000),24)=result;snapshot();if(mode)require(step==count&&calls==1&&result==output);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_TEXTURE_PIXEL_FORMAT_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
