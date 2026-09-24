#include "fable_ui_texture_surfaces.h"
#include <stdio.h>
extern "C" __declspec(dllimport) void* __stdcall LoadLibraryA(const char*);
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void*,const char*);
extern "C" __declspec(dllimport) int __stdcall FreeLibrary(void*);
extern "C" __declspec(dllimport) void* __stdcall GetModuleHandleA(const char*);
typedef void* (__stdcall *CreateWindow)(unsigned,const char*,const char*,unsigned,int,int,int,int,void*,void*,void*,void*);
typedef int (__stdcall *DestroyWindow)(void*);
struct PresentParameters
{
    unsigned Width,Height,Format,Count,MultiSample,Quality,SwapEffect;
    void* Window;
    int Windowed,AutoDepth;
    unsigned DepthFormat,Flags,RefreshRate,Interval;
};
typedef void* (__stdcall *CreateD3D)(unsigned);
typedef long (__stdcall *CreateDevice)(void*,unsigned,unsigned,void*,unsigned,PresentParameters*,void**);
typedef unsigned (__stdcall *ReleaseCom)(void*);
static void Release(void* p) { if(p) reinterpret_cast<ReleaseCom>((*static_cast<void***>(p))[2])(p); }
struct DeviceSession
{
    void* User; void* D3DLibrary; void* Window; void* D3D; void* Device; DestroyWindow Destroy;
    DeviceSession() : User(0),D3DLibrary(0),Window(0),D3D(0),Device(0),Destroy(0) {}
    ~DeviceSession() { Release(Device); Release(D3D); if(Window && Destroy) Destroy(Window); if(D3DLibrary) FreeLibrary(D3DLibrary); if(User) FreeLibrary(User); }
};
void* FableUiSurfaceVtable=reinterpret_cast<void*>(0x70000108);
FableUiPixelFormatDescription FableUiPixelFormats[64];
FableUiSystemManagerView FableUiSystemManagerState;
static FableUiDisplayFormatView display;
static int CheckTexture(unsigned format,unsigned width,unsigned height)
{
    CTexture texture={}; int index=static_cast<int>(format);
    FableUiDisplayExtent dimensions={static_cast<int>(width),static_cast<int>(height)};
    if(!FableUiCreateBlankTexture(&texture,0,&dimensions,-1,&index,0,1,false,0))
    { printf("CREATE_FAIL format=%u width=%u height=%u\n",FableUiPixelFormats[format].D3DFormat,width,height); FableUiReleaseTexture(&texture,0); return 1; }
    unsigned levels=1,w=width,h=height; while(w>=8 && h>=8) { w>>=1; h>>=1; ++levels; }
    unsigned expectedBytes=0; w=width; h=height;
    for(unsigned i=0;i<levels;++i) { expectedBytes+=w*h*FableUiPixelFormats[format].Bits/8; w>>=1; h>>=1; }
    if(FableUiTextureState(&texture)!=(0x10000000|expectedBytes))
    { printf("SIZE_FAIL actual=%08x expected=%08x\n",FableUiTextureState(&texture),0x10000000|expectedBytes); FableUiReleaseTexture(&texture,0); return 2; }
    CSurface surface; memset(&surface,0xA5,sizeof(surface)); FableUiGetTextureSurface(&texture,0,&surface,0);
    FableUiSurfaceLock lock; FableUiLockSurface(&surface,0,&lock,0);
    if(!lock.Bits || lock.Width!=width || lock.Height!=height)
    { printf("LOCK_FAIL\n"); if(lock.Bits) { FableUiNativeSurface* p=FableUiNative(&surface); p->Vtable->Unlock(p); } FableUiReleaseSurface(&surface,0); FableUiReleaseTexture(&texture,0); return 3; }
    unsigned rowBytes=width*FableUiPixelFormats[format].Bits/8;
    for(i=0;i<height;++i) memset(static_cast<unsigned char*>(lock.Bits)+i*lock.Pitch,0xA7,rowBytes);
    FableUiNativeSurface* native=FableUiNative(&surface); native->Vtable->Unlock(native);
    FableUiClearSurface(&surface,0);
    FableUiLockSurface(&surface,0,&lock,0);
    if(!lock.Bits) { FableUiReleaseSurface(&surface,0); FableUiReleaseTexture(&texture,0); return 4; }
    unsigned nonzero=0;
    for(i=0;i<height;++i) for(unsigned x=0;x<rowBytes;++x) nonzero+=static_cast<unsigned char*>(lock.Bits)[i*lock.Pitch+x]!=0;
    native=FableUiNative(&surface); native->Vtable->Unlock(native);
    printf("TEXTURE format=%u dimensions=%ux%u levels=%u bytes=%u pitch=%d uncleared=%u\n",FableUiPixelFormats[format].D3DFormat,width,height,levels,expectedBytes,lock.Pitch,nonzero);
    FableUiReleaseSurface(&surface,0); FableUiReleaseTexture(&texture,0);
    return nonzero ? 5 : 0;
}
int main()
{
    DeviceSession session;
    session.User=LoadLibraryA("user32.dll"); session.D3DLibrary=LoadLibraryA("d3d9.dll");
    if(!session.User || !session.D3DLibrary) { printf("DEVICE_UNAVAILABLE libraries\n"); return 10; }
    CreateWindow create=reinterpret_cast<CreateWindow>(GetProcAddress(session.User,"CreateWindowExA"));
    session.Destroy=reinterpret_cast<DestroyWindow>(GetProcAddress(session.User,"DestroyWindow"));
    CreateD3D createD3D=reinterpret_cast<CreateD3D>(GetProcAddress(session.D3DLibrary,"Direct3DCreate9"));
    if(!create || !session.Destroy || !createD3D) return 11;
    // Owned hidden window: no ShowWindow, message loop, fullscreen switch or presenter.
    session.Window=create(0,"STATIC","Fable native texture verification",0x80000000,0,0,64,64,0,0,GetModuleHandleA(0),0);
    session.D3D=createD3D(32); if(!session.Window || !session.D3D) { printf("DEVICE_UNAVAILABLE initialization\n"); return 12; }
    PresentParameters parameters={}; parameters.Width=64; parameters.Height=64; parameters.Count=1; parameters.SwapEffect=1; parameters.Window=session.Window; parameters.Windowed=1;
    CreateDevice createDevice=reinterpret_cast<CreateDevice>((*static_cast<void***>(session.D3D))[16]);
    long hr=createDevice(session.D3D,0,1,session.Window,0x20,&parameters,&session.Device);
    if(hr<0 || !session.Device) { printf("DEVICE_UNAVAILABLE HRESULT=%08lx\n",hr); return 13; }
    display.D3DDevice=session.Device; FableUiSystemManagerState.Display=&display;
    unsigned formats[3]={21,22,23}; int bits[3]={32,32,16};
    for(unsigned i=0;i<3;++i) { FableUiPixelFormats[i].D3DFormat=formats[i]; FableUiPixelFormats[i].Bits=bits[i]; }
    FableUiPixelFormats[3].Bits=-1;
    unsigned dimensions[3][2]={{32,32},{64,32},{16,64}};
    unsigned failures=0;
    for(i=0;i<3;++i) for(unsigned d=0;d<3;++d) failures+=CheckTexture(i,dimensions[d][0],dimensions[d][1])!=0;
    printf("UI_TEXTURE_DEVICE %s cases=9 failures=%u\n",failures ? "FAIL" : "PASS",failures);
    return failures ? 1 : 0;
}
