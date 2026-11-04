#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
const CD3DFormatInfo GD3DFormats[]={
 {static_cast<D3DFORMAT>(100),PF_TYPE_RGBA,32,0,0,0,0,8,false},
 {static_cast<D3DFORMAT>(101),PF_TYPE_DEPTH,16,0,0,0,0,7,false},
 {static_cast<D3DFORMAT>(102),PF_TYPE_DEPTH,16,0,0,0,0,8,false},
 {static_cast<D3DFORMAT>(103),PF_TYPE_DEPTH,24,0,0,0,0,8,false},
 {static_cast<D3DFORMAT>(104),PF_TYPE_DEPTH,32,0,0,0,0,8,false},
 {static_cast<D3DFORMAT>(105),PF_TYPE_DEPTH,24,0,0,0,0,16,false},
 {static_cast<D3DFORMAT>(106),PF_TYPE_DEPTH,40,0,0,0,0,8,false},
 {D3DFMT_UNKNOWN,PF_TYPE_NULL,-1,0,0,0,0,0,false}};
static unsigned long storage[0x2c8/4],table[17],api;
static unsigned errors,formatMask,matchMask,mutation,formatCalls,matchCalls,initialiseCalls;
static D3DDISPLAYMODE mode;
static CDisplayManager* Object(){return reinterpret_cast<CDisplayManager*>(storage);}
struct CDisplayManagerTestAccess {static CPixelFormat Closest(long depth){return Object()->GetClosestDepthBufferFormat(mode,depth);}};
void CPixelFormat::Initialise(D3DFORMAT format){++initialiseCalls;FormatIndex=static_cast<long>(format)-100;if(FormatIndex<2 || FormatIndex>6)++errors;}
static bool Fails(unsigned mask,unsigned index){return index>=3 && index<=5 && (mask&(1U<<(index-3)))!=0;}
static void Check(void* self,UINT adapter,D3DDEVTYPE type,D3DFORMAT format){if(self!=&api || adapter!=Object()->D3DDeviceCaps.AdapterOrdinal || type!=Object()->D3DDeviceCaps.DeviceType || format!=mode.Format)++errors;}
static HRESULT FABLE_STDCALL Format(IDirect3D9* self,UINT adapter,D3DDEVTYPE type,D3DFORMAT format,DWORD usage,D3DRESOURCETYPE resource,D3DFORMAT target){
 Check(self,adapter,type,format);unsigned index=static_cast<unsigned>(target)-100;if(index<2 || index>6 || usage!=2 || resource!=D3DRTYPE_SURFACE)++errors;++formatCalls;
 if(mutation){mode.Format=static_cast<D3DFORMAT>(42);Object()->D3DDeviceCaps.AdapterOrdinal=9;}
 return Fails(formatMask,index)?static_cast<HRESULT>(0x80004005):static_cast<HRESULT>(index&1);
}
static HRESULT FABLE_STDCALL Match(IDirect3D9* self,UINT adapter,D3DDEVTYPE type,D3DFORMAT format,D3DFORMAT render,D3DFORMAT depth){
 Check(self,adapter,type,format);unsigned index=static_cast<unsigned>(depth)-100;if(render!=mode.Format || index<2 || index>6 || Fails(formatMask,index))++errors;++matchCalls;return Fails(matchMask,index)?static_cast<HRESULT>(0x80004005):static_cast<HRESULT>(index&1);
}
int main(){table[10]=reinterpret_cast<unsigned long>(&Format);table[12]=reinterpret_cast<unsigned long>(&Match);api=reinterpret_cast<unsigned long>(table);unsigned scenarios=0;const long targets[]={-32,0,8,16,20,24,28,32,36,40,64,128};
 for(unsigned t=0;t<12;++t)for(formatMask=0;formatMask<8;++formatMask)for(matchMask=0;matchMask<8;++matchMask)for(mutation=0;mutation<2;++mutation){
  memset(storage,0,sizeof(storage));Object()->D3D=reinterpret_cast<IDirect3D9*>(&api);Object()->D3DDeviceCaps.AdapterOrdinal=7;Object()->D3DDeviceCaps.DeviceType=static_cast<D3DDEVTYPE>(1);mode.Width=640;mode.Height=480;mode.RefreshRate=60;mode.Format=D3DFMT_X8R8G8B8;formatCalls=matchCalls=initialiseCalls=0;
  CPixelFormat result=CDisplayManagerTestAccess::Closest(targets[t]);long best=-1,bestDistance=LONG_MAX;unsigned matches=0;
  for(unsigned i=2;i<=6;++i){if(Fails(formatMask,i))continue;++matches;if(Fails(matchMask,i))continue;long distance=abs(targets[t]-GD3DFormats[i].BitDepth);if(distance<bestDistance || (distance==bestDistance && targets[t]<GD3DFormats[i].BitDepth)){best=i;bestDistance=distance;}}
  if(result.FormatIndex!=best || formatCalls!=5 || matchCalls!=matches || initialiseCalls!=1)++errors;++scenarios;
 }
 if(errors){printf("DISPLAY_DEPTH_FORMAT_FAIL errors=%u\n",errors);return 1;}printf("DISPLAY_DEPTH_FORMAT_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
