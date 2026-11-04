#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
const CD3DFormatInfo GD3DFormats[]={
 {D3DFMT_R8G8B8,PF_TYPE_RGBA,16,0,0,0,0,0,false},
 {D3DFMT_A8R8G8B8,PF_TYPE_RGBA,24,0,0,0,0,0,false},
 {D3DFMT_X8R8G8B8,PF_TYPE_RGBA,32,0,0,0,0,0,false}};
static unsigned long storage[0x2c8/4],apiTable[17],deviceTable[119],api,device;
static CPixelFormat formats[3];
static unsigned errors,profile,order,mutation,failure,colourCalls,enumCalls,countCalls,displayCalls;
static C2DExtentsI request;
static CDisplayManager* Object(){return reinterpret_cast<CDisplayManager*>(storage);}
struct CDisplayManagerTestAccess {static D3DDISPLAYMODE Closest(long depth,bool exclusive){return Object()->GetClosestMode(request,depth,exclusive);}};
static unsigned Count(unsigned format){return profile==0?0:(profile+format)%4+1;}
static D3DDISPLAYMODE Mode(unsigned format,unsigned index){
 const unsigned widths[]={640,800,1024,1280},heights[]={480,600,768,720};
 unsigned slot=profile==5?1:(profile+format+index)%4;D3DDISPLAYMODE mode;
 mode.Width=widths[slot];mode.Height=heights[slot];mode.RefreshRate=50+index+format*5;mode.Format=static_cast<D3DFORMAT>(20+format);return mode;
}
void CPixelFormat::Initialise(D3DFORMAT format){FormatIndex=static_cast<long>(format)-20;if(FormatIndex<0 || FormatIndex>2)++errors;}
unsigned long CPixelFormat::GetColourDepth() const {++colourCalls;if(mutation){request.X=800;request.Y=600;}return 16+8*FormatIndex;}
static unsigned FABLE_STDCALL ModeCount(IDirect3D9* self,UINT adapter,D3DFORMAT format){if(self!=reinterpret_cast<IDirect3D9*>(&api) || adapter!=7 || format<20 || format>22)++errors;++countCalls;return Count(format-20);}
static HRESULT FABLE_STDCALL EnumMode(IDirect3D9* self,UINT adapter,D3DFORMAT format,UINT index,D3DDISPLAYMODE* out){if(self!=reinterpret_cast<IDirect3D9*>(&api) || adapter!=7 || index>=Count(format-20))++errors;++enumCalls;*out=Mode(format-20,index);return failure?static_cast<HRESULT>(0x80004005):0;}
static HRESULT FABLE_STDCALL DisplayMode(void* self,UINT adapter,D3DDISPLAYMODE* out){if((self==&api && adapter!=7) || (self==&device && adapter!=0) || (self!=&api && self!=&device))++errors;++displayCalls;*out=Mode(2,0);if(mutation){request.X=800;request.Y=600;}return failure?static_cast<HRESULT>(0x80004005):0;}
static void Fill(unsigned count,bool haveDevice){
 memset(storage,0,sizeof(storage));Object()->D3D=reinterpret_cast<IDirect3D9*>(&api);Object()->D3DDevice=haveDevice?reinterpret_cast<IDirect3DDevice9*>(&device):0;Object()->D3DDeviceCaps.AdapterOrdinal=7;
 for(unsigned i=0;i<3;++i)formats[i].FormatIndex=order?2-i:i;
 Object()->BackBufferFormats._M_start=formats;Object()->BackBufferFormats._M_finish=formats+count;Object()->BackBufferFormats._M_end_of_storage._M_data=formats+3;
 colourCalls=enumCalls=countCalls=displayCalls=0;
}
int main(){apiTable[6]=reinterpret_cast<unsigned long>(&ModeCount);apiTable[7]=reinterpret_cast<unsigned long>(&EnumMode);apiTable[8]=deviceTable[8]=reinterpret_cast<unsigned long>(&DisplayMode);api=reinterpret_cast<unsigned long>(apiTable);device=reinterpret_cast<unsigned long>(deviceTable);unsigned scenarios=0;
 for(profile=0;profile<6;++profile)for(unsigned requested=0;requested<3;++requested)for(unsigned depth=0;depth<3;++depth)for(order=0;order<2;++order)for(mutation=0;mutation<2;++mutation)for(failure=0;failure<2;++failure){
  unsigned count=profile%4;Fill(count,false);request.X=640+requested*160;request.Y=480+requested*120;long bitDepth=16+depth*8;
  D3DDISPLAYMODE actual=CDisplayManagerTestAccess::Closest(bitDepth,true),expected;expected.Format=D3DFMT_UNKNOWN;
  long bestDepth=LONG_MAX,bestDistance=LONG_MAX;unsigned visited=0,expectedCounts=0;
  for(unsigned f=0;f<count;++f){unsigned format=order?2-f:f;expectedCounts+=Count(format)+1;for(unsigned m=0;m<Count(format);++m){++visited;D3DDISPLAYMODE candidate=Mode(format,m);long difference=abs(16+8*static_cast<long>(format)-bitDepth);long distance=abs(static_cast<long>(candidate.Width)-request.X)+abs(static_cast<long>(candidate.Height)-request.Y);if(difference<=bestDepth && distance<=bestDistance){expected=candidate;bestDepth=difference;bestDistance=distance;}}}
  if(actual.Format!=expected.Format || (visited && memcmp(&actual,&expected,16)) || enumCalls!=visited || colourCalls!=visited || countCalls!=expectedCounts || displayCalls)++errors;++scenarios;
 }
 profile=3;
 for(unsigned haveDevice=0;haveDevice<2;++haveDevice)for(unsigned dims=0;dims<3;++dims)for(mutation=0;mutation<2;++mutation)for(failure=0;failure<2;++failure){
  Fill(3,haveDevice!=0);request.X=300+dims*200;request.Y=400+dims*100;D3DDISPLAYMODE actual=CDisplayManagerTestAccess::Closest(24,false),expected=Mode(2,0);expected.Width=request.X;expected.Height=request.Y;
  if(memcmp(&actual,&expected,16) || colourCalls || enumCalls || countCalls || displayCalls!=1)++errors;++scenarios;
 }
 if(errors){printf("DISPLAY_CLOSEST_MODE_FAIL errors=%u\n",errors);return 1;}printf("DISPLAY_CLOSEST_MODE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
