#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_volume_types.h"
#include "fable_d3d_format_info.h"
#include "volume_texture_dimensions_retail.h"
#include "volume_texture_dimensions_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" C3DExtentsI* __fastcall VolumeTextureDimensions(void*,void*,C3DExtentsI*,unsigned long);
static unsigned arena[4096],initial[4096],images[3][4096],mode,step,count,snapshots,location,formatProfile,profile,mutation,calls,output,levelProfile;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void snapshot(){if(!mode){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static long __stdcall levelDesc(void* texture,unsigned long level,D3DVOLUME_DESC* description){require(level==pattern(levelProfile));require(!calls&&(unsigned)texture==ptr(0x400));++calls;memset(description,0xa9,sizeof(*description));description->Format=(D3DFORMAT)pattern(formatProfile);description->Width=pattern(profile+mutation)^level;description->Height=pattern(profile+3);description->Depth=pattern(profile+5);word(ptr(0x2000),0)=calls;word(ptr(0x2000),4)=(unsigned)texture;word(ptr(0x2000),8)=description->Depth;word(ptr(0x2000),12)=description->Width;word(ptr(0x2000),16)=description->Height;word(ptr(0x2000),20)=(unsigned)description->Format;if(mutation){word(ptr(0x100),4)=ptr(0x800);word(ptr(0x100),0)^=0xffffffff;word(output,0)=pattern(profile+2);}snapshot();return mutation==3?(long)0x80004005:0;}
static unsigned model(){D3DVOLUME_DESC description;levelDesc((void*)word(ptr(0x100),4),pattern(levelProfile),&description);word(output,0)=description.Width;word(output,4)=description.Height;word(output,8)=description.Depth;return output;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned table[18]={0};table[17]=(unsigned)levelDesc;const unsigned outputs[4]={0x600,0x100,0x104,0x400};unsigned cases=0;
for(levelProfile=0;levelProfile<8;++levelProfile)for(formatProfile=0;formatProfile<8;++formatProfile)for(profile=0;profile<8;++profile)for(unsigned out=0;out<4;++out)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;output=ptr(outputs[out]);memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),0)=pattern(profile);word(ptr(0x100),4)=ptr(0x400);word(ptr(0x400),0)=word(ptr(0x800),0)=(unsigned)table;memset((void*)ptr(0x2000),0,28);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=calls=0;unsigned result=!mode?model():(unsigned)(mode==1?((C3DExtentsI*(__fastcall*)(void*,void*,C3DExtentsI*,unsigned long))code):VolumeTextureDimensions)((void*)ptr(0x100),0,(C3DExtentsI*)output,pattern(levelProfile));word(ptr(0x2000),24)=result;snapshot();if(mode)require(step==count&&calls==1&&result==output);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_DIMENSIONS_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
