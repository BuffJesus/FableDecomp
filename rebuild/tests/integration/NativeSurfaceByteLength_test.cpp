#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_d3d9.h"
#include "fable_d3d_format_info.h"
#include "surface_byte_length_retail.h"
#include "surface_byte_length_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned long __fastcall SurfaceByteLength(void*,void*);
static unsigned arena[4096],initial[4096],images[3][4096],vtable[16];
static unsigned row,width,height,mutation,hr,mode,calls,snapshots;static unsigned char* owner;static unsigned* firstInterface;static unsigned* secondInterface;static D3DSURFACE_DESC* firstDesc;
static void require(bool b){if(!b)abort();}
static unsigned endpoint(unsigned i){const unsigned values[]={0,1,2,3,0xffff,0x7fffffff,0x80000000,0xffffffff};return values[i&7];}
static void snapshot(unsigned at){if(!mode)memcpy(images[at],arena,sizeof(arena));else{require(!memcmp(images[at],arena,sizeof(arena)));++snapshots;}require(!memcmp(GD3DFormats,retailFormats,sizeof(retailFormats)));}
static long __stdcall getDesc(void* self,D3DSURFACE_DESC* out){require(calls<2);require(self==(calls==1&&mutation==1?(void*)secondInterface:(void*)firstInterface));memset(out,0xa5,sizeof(*out));out->Format=(D3DFORMAT)retailFormats[((row+(calls?0:1))%46)*9];out->Width=endpoint(width+(calls?3:0));out->Height=endpoint(height+(calls?4:0));if(!calls){firstDesc=out;if(mutation==1)*(unsigned*)(owner+4)=(unsigned)secondInterface;}else{if(mutation==2){firstDesc->Width=endpoint(width+1);firstDesc->Height=endpoint(height+1);}if(mutation==3){*(unsigned*)(owner+4)=(unsigned)secondInterface;*(unsigned*)(owner+8)=0x13579bdf;*(unsigned*)(owner+12)=0x2468ace0;}}arena[0x2200/4+calls]=(self==firstInterface?0x10:0x20)|calls;++calls;snapshot(calls-1);return hr?(long)0x80004005:0;}
static unsigned model(){D3DSURFACE_DESC dimensions,format;getDesc((void*)*(unsigned*)(owner+4),&dimensions);getDesc((void*)*(unsigned*)(owner+4),&format);unsigned index=0;while(retailFormats[index*9]!=(unsigned)format.Format){++index;require(retailFormats[index*9+2]!=0xffffffff);}unsigned bytes=retailFormats[index*9+2];bytes*=dimensions.Height;bytes*=dimensions.Width;return bytes/8;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));for(unsigned i=0;i<sizeof(fixups)/sizeof(*fixups);++i){unsigned target=fixups[i].target&0x80000000?(unsigned)GD3DFormats+(fixups[i].target&0x7fffffff):(unsigned)code+fixups[i].target;*(unsigned*)(code+fixups[i].site)=fixups[i].kind==20?target-(unsigned)(code+fixups[i].site+4):target;}require(sizeof(fixups)/sizeof(*fixups)==4);memset(vtable,0,sizeof(vtable));vtable[12]=(unsigned)getDesc;unsigned cases=0;
for(row=0;row<46;++row)for(width=0;width<8;++width)for(height=0;height<8;++height)for(hr=0;hr<2;++hr)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){memset(arena,fill?0xa9:0,sizeof(arena));owner=(unsigned char*)arena+(place?0x1800:0)+0x100;firstInterface=(unsigned*)(owner+0x200);secondInterface=(unsigned*)(owner+0x300);firstInterface[0]=secondInterface[0]=(unsigned)vtable;*(unsigned*)(owner+4)=(unsigned)firstInterface;memcpy(initial,arena,sizeof(arena));unsigned expected=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));calls=0;firstDesc=0;unsigned result=mode==0?model():mode==1?((unsigned(__fastcall*)(void*,void*))code)(owner,0):SurfaceByteLength(owner,0);require(calls==2);if(!mode)expected=result;else require(result==expected);arena[0x2300/4]=result;snapshot(2);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_SURFACE_BYTE_LENGTH_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
