#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include "fable_exception.h"
#include <stdio.h>
#include <string.h>
const CD3DFormatInfo GD3DFormats[]={
 {D3DFMT_X8R8G8B8,PF_TYPE_RGBA,32,0,0,0,0,0,false},
 {static_cast<D3DFORMAT>(75),PF_TYPE_DEPTH,24,0,0,0,0,8,false}};
static unsigned long storage[0x2c8/4],parameterStorage[11],apiTable[17],deviceTable[119],api,device,surfaces[2];
static unsigned errors,events[40],eventCount,checkResult,resetResult,failStage,mutation;
static bool aliasParameters;
static CDisplayManager::CCreateDeviceParameters* input;
static unsigned char copied[44];
static CDisplayManager* Object(){return reinterpret_cast<CDisplayManager*>(storage);}
static void Event(unsigned id){if(eventCount>=40){++errors;return;}events[eventCount++]=id;}
static unsigned Offset(const void* p){return static_cast<unsigned>(reinterpret_cast<const char*>(p)-reinterpret_cast<const char*>(Object()));}
struct CDisplayManagerTestAccess {static unsigned Reset(){return Object()->ResetDevice(*input);}};
void CDisplayManager::PreDeviceReset(){if(this!=Object() || memcmp(&LastDeviceCreationParameters,copied,44))++errors;Event(1);if(mutation&1){input->BackBufferCount=3;input->RefreshRate=75;input->PresentationInterval.VSyncCount=2;}}
void CSurface::Uninitialise(){Event(Offset(this));PD3DSurface=0;AllocationSource=0;PAllocatedMemory=0;}
void CTexture::Uninitialise(){Event(Offset(this));PD3DTexture=0;ByteLength=0;AllocationSource=0;}
D3DDISPLAYMODE CDisplayManager::GetClosestMode(const C2DExtentsI& dimensions,long depth,bool exclusive) const {
 if(this!=Object() || &dimensions!=&input->BackBufferDimensions || depth!=input->BackBufferBitDepth || exclusive!=input->ExclusiveMode)++errors;Event(2);D3DDISPLAYMODE out={800,600,60,D3DFMT_X8R8G8B8};return out;
}
void CPixelFormat::Initialise(D3DFORMAT format){if(format!=D3DFMT_X8R8G8B8)++errors;Event(3);FormatIndex=0;if(mutation&2)input->MultisampleType=SURFACE_MULTISAMPLE_4;}
CPixelFormat CDisplayManager::GetClosestDepthBufferFormat(const D3DDISPLAYMODE& mode,long bits) const {
 if(this!=Object() || &mode!=&CurrentMode || bits!=input->ZBufferBitDepth || mode.Width!=800 || mode.Height!=600 || mode.Format!=D3DFMT_X8R8G8B8)++errors;Event(5);CPixelFormat out;out.FormatIndex=1;return out;
}
unsigned CDisplayManager::CalculateD3DPresentationInterval(const CPresentationIntervalDesc& desc){if(this!=Object() || &desc!=&PresentationInterval || memcmp(&desc,&input->PresentationInterval,8))++errors;Event(6);return 4;}
static HRESULT FABLE_STDCALL CheckMulti(IDirect3D9* self,UINT adapter,D3DDEVTYPE type,D3DFORMAT format,BOOL windowed,D3DMULTISAMPLE_TYPE sample,DWORD* quality){
 if(self!=reinterpret_cast<IDirect3D9*>(&api) || adapter || type!=Object()->D3DDeviceCaps.DeviceType || format!=D3DFMT_X8R8G8B8 || windowed || static_cast<unsigned>(sample)!=input->MultisampleType || quality)++errors;Event(4);return checkResult==2?static_cast<HRESULT>(0x80004005):checkResult;
}
static HRESULT FABLE_STDCALL Reset(IDirect3DDevice9* self,D3DPRESENT_PARAMETERS* p){
 if(self!=reinterpret_cast<IDirect3DDevice9*>(&device) || p!=&Object()->PresentParams)++errors;
 D3DPRESENT_PARAMETERS expected;memset(&expected,0,sizeof(expected));expected.BackBufferWidth=800;expected.BackBufferHeight=600;expected.BackBufferFormat=D3DFMT_X8R8G8B8;expected.BackBufferCount=input->BackBufferCount;expected.MultiSampleType=static_cast<D3DMULTISAMPLE_TYPE>(input->ExclusiveMode && input->MultisampleType && checkResult==0?input->MultisampleType:0);expected.SwapEffect=D3DSWAPEFFECT_DISCARD;expected.hDeviceWindow=Object()->WindowHandle;expected.Windowed=!input->ExclusiveMode;expected.EnableAutoDepthStencil=1;expected.AutoDepthStencilFormat=static_cast<D3DFORMAT>(75);expected.FullScreen_RefreshRateInHz=input->ExclusiveMode?input->RefreshRate:0;expected.PresentationInterval=input->ExclusiveMode?4:0;
 if(memcmp(p,&expected,sizeof(expected)) || Object()->CurrentScreenIndex || Object()->NoBackBuffers!=input->BackBufferCount || Object()->MultisampleType!=static_cast<ESurfaceMultisampleType>(expected.MultiSampleType))++errors;
 Event(7);return resetResult==2?D3DERR_DEVICELOST:resetResult==3?static_cast<HRESULT>(0x80004005):resetResult;
}
static HRESULT FABLE_STDCALL AdapterMode(IDirect3D9* self,UINT adapter,D3DDISPLAYMODE* mode){if(self!=reinterpret_cast<IDirect3D9*>(&api) || adapter || Object()->DeviceLostFlag)++errors;Event(8);mode->Width=1600;mode->Height=900;mode->RefreshRate=50;mode->Format=D3DFMT_X8R8G8B8;return failStage==1?static_cast<HRESULT>(0x80004005):1;}
static HRESULT FABLE_STDCALL BackBuffer(IDirect3DDevice9* self,UINT swap,UINT back,int type,IDirect3DSurface9** out){volatile float expectedTime=static_cast<float>(input->PresentationInterval.VSyncCount)/50.0f;if(self!=reinterpret_cast<IDirect3DDevice9*>(&device) || swap || back || type || Object()->RefreshRate!=50 || Object()->PresentationIntervalTime!=expectedTime)++errors;Event(9);*out=reinterpret_cast<IDirect3DSurface9*>(&surfaces[0]);return failStage==2?static_cast<HRESULT>(0x80004005):1;}
static HRESULT FABLE_STDCALL DepthBuffer(IDirect3DDevice9* self,IDirect3DSurface9** out){if(self!=reinterpret_cast<IDirect3DDevice9*>(&device) || Object()->BackBufferDimensionsStorage_[0]!=1280 || Object()->BackBufferDimensionsStorage_[1]!=720 || Object()->BackBufferFormatStorage_!=0)++errors;Event(13);*out=reinterpret_cast<IDirect3DSurface9*>(&surfaces[1]);return failStage==3?static_cast<HRESULT>(0x80004005):1;}
void CSurface::InitialiseFromPreallocatedSurface(IDirect3DSurface9* surface,bool addReference){unsigned offset=Offset(this);if(addReference || (offset!=0x2c && offset!=0x3c) || surface!=reinterpret_cast<IDirect3DSurface9*>(&surfaces[offset==0x2c?0:1]))++errors;Event(offset==0x2c?10:14);PD3DSurface=surface;}
C2DExtentsI CSurface::GetDimensions() const {if(Offset(this)!=0x2c)++errors;Event(11);return C2DExtentsI(1280,720);}
CPixelFormat CSurface::GetPixelFormat() const {unsigned offset=Offset(this);if(offset!=0x2c && offset!=0x3c)++errors;Event(offset==0x2c?12:15);CPixelFormat out;out.FormatIndex=offset==0x2c?0:1;return out;}
CSurface& CSurface::operator=(const CSurface& source){unsigned offset=Offset(this);if((offset!=0xc && offset!=0x1c) || Offset(&source)!=offset+0x20 || Object()->DepthBufferFormatStorage_!=1)++errors;Event(offset==0xc?16:17);PD3DSurface=source.PD3DSurface;AllocationSource=source.AllocationSource;PAllocatedMemory=source.PAllocatedMemory;return *this;}
bool CDisplayManager::SetupDepthAndBackBufferTextures(){if(this!=Object() || ExclusiveMode!=input->ExclusiveMode || RenderTargetDimensionsStorage_[0]!=1280 || RenderTargetDimensionsStorage_[1]!=720)++errors;Event(18);return failStage!=4;}
bool CDisplayManager::PostDeviceReset(){if(this!=Object())++errors;Event(19);return failStage!=5;}
int main(){apiTable[8]=reinterpret_cast<unsigned long>(&AdapterMode);apiTable[11]=reinterpret_cast<unsigned long>(&CheckMulti);deviceTable[16]=reinterpret_cast<unsigned long>(&Reset);deviceTable[18]=reinterpret_cast<unsigned long>(&BackBuffer);deviceTable[40]=reinterpret_cast<unsigned long>(&DepthBuffer);api=reinterpret_cast<unsigned long>(apiTable);device=reinterpret_cast<unsigned long>(deviceTable);unsigned scenarios=0;
 for(unsigned exclusive=0;exclusive<2;++exclusive)for(unsigned sample=0;sample<3;++sample)for(checkResult=0;checkResult<3;++checkResult)for(resetResult=0;resetResult<4;++resetResult)for(failStage=0;failStage<6;++failStage)for(mutation=0;mutation<4;++mutation)for(unsigned alias=0;alias<2;++alias){
  aliasParameters=alias!=0;memset(storage,0,sizeof(storage));input=aliasParameters?&Object()->LastDeviceCreationParameters:reinterpret_cast<CDisplayManager::CCreateDeviceParameters*>(parameterStorage);memset(input,0xa5,44);input->WinHandle=reinterpret_cast<HWND__*>(0x1111);input->BackBufferDimensions.X=640;input->BackBufferDimensions.Y=480;input->BackBufferBitDepth=32;input->ZBufferBitDepth=24;input->BackBufferCount=2;input->RefreshRate=60;input->ExclusiveMode=exclusive!=0;input->PresentationInterval.PresentImmediate=false;input->PresentationInterval.PresentImmediateIfSyncMissed=true;input->PresentationInterval.VSyncCount=1;input->MultisampleType=static_cast<ESurfaceMultisampleType>(sample*2);memcpy(copied,input,44);
  Object()->D3D=reinterpret_cast<IDirect3D9*>(&api);Object()->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(&device);Object()->D3DDeviceCaps.DeviceType=static_cast<D3DDEVTYPE>(1);Object()->WindowHandle=reinterpret_cast<HWND__*>(0x2222);Object()->DeviceLostFlag=true;Object()->CurrentScreenIndex=4;eventCount=0;
  unsigned result=99;bool threw=false;try{result=CDisplayManagerTestAccess::Reset();}catch(const CGenericException&){threw=true;}
  unsigned expectedEvents[40],n=0;expectedEvents[n++]=1;expectedEvents[n++]=0x2c;expectedEvents[n++]=0x3c;expectedEvents[n++]=0xc;expectedEvents[n++]=0x1c;expectedEvents[n++]=0x4c;expectedEvents[n++]=2;if(exclusive && sample){expectedEvents[n++]=3;expectedEvents[n++]=4;}expectedEvents[n++]=5;if(exclusive)expectedEvents[n++]=6;expectedEvents[n++]=7;
  if(resetResult<2){expectedEvents[n++]=8;if(failStage!=1){expectedEvents[n++]=9;if(failStage!=2){expectedEvents[n++]=10;expectedEvents[n++]=11;expectedEvents[n++]=12;expectedEvents[n++]=13;if(failStage!=3){expectedEvents[n++]=14;expectedEvents[n++]=15;expectedEvents[n++]=16;expectedEvents[n++]=17;expectedEvents[n++]=18;if(failStage!=4)expectedEvents[n++]=19;}}}}
  unsigned expectedResult=resetResult==2?1:resetResult==3?0:failStage?0:2;bool expectedThrow=resetResult<2 && failStage==1;
  if(threw!=expectedThrow || (!threw && result!=expectedResult) || eventCount!=n || memcmp(events,expectedEvents,n*sizeof(unsigned)) || Object()->DeviceLostFlag!=(resetResult==2))++errors;
  if(!aliasParameters && memcmp(&Object()->LastDeviceCreationParameters,copied,44))++errors;
  if(errors){printf("DISPLAY_RESET_FAIL scenario=%u errors=%u\n",scenarios,errors);return 1;}++scenarios;
 }
 printf("DISPLAY_RESET_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
