#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include "fable_exception.h"
#include <string.h>

CDisplayManager::ECreateDeviceSuccess CDisplayManager::CreateDevice(const CCreateDeviceParameters& parameters)
{
    LastDeviceCreationParameters=parameters;
    CurrentMode=GetClosestMode(parameters.BackBufferDimensions,parameters.BackBufferBitDepth,parameters.ExclusiveMode);
    WindowHandle=parameters.WinHandle;
    NoBackBuffers=parameters.BackBufferCount;
    ExclusiveMode=parameters.ExclusiveMode;
    CurrentScreenIndex=0;
    PresentationInterval=parameters.PresentationInterval;
    memset(&PresentParams,0,sizeof(PresentParams));
    PresentParams.BackBufferWidth=CurrentMode.Width;
    PresentParams.BackBufferHeight=CurrentMode.Height;
    PresentParams.BackBufferFormat=CurrentMode.Format;
    PresentParams.SwapEffect=D3DSWAPEFFECT_DISCARD;
    PresentParams.EnableAutoDepthStencil=1;
    PresentParams.Windowed=!parameters.ExclusiveMode;
    PresentParams.AutoDepthStencilFormat=GD3DFormats[GetClosestDepthBufferFormat(CurrentMode,parameters.ZBufferBitDepth).FormatIndex].D3DFormat;
    PresentParams.hDeviceWindow=WindowHandle;
    PresentParams.BackBufferCount=parameters.BackBufferCount;
    if (ExclusiveMode)
        PresentParams.FullScreen_RefreshRateInHz=parameters.RefreshRate;
    else
        PresentParams.FullScreen_RefreshRateInHz=0;
    unsigned int interval=CalculateD3DPresentationInterval(PresentationInterval);
    DWORD flags=(D3DDeviceCaps.DevCaps&D3DDEVCAPS_PUREDEVICE)
        ? D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_PUREDEVICE|D3DCREATE_FPU_PRESERVE|D3DCREATE_MULTITHREADED
        : D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_FPU_PRESERVE|D3DCREATE_MULTITHREADED;
    PresentParams.PresentationInterval=interval;
    if (parameters.MultisampleType!=SURFACE_MULTISAMPLE_NONE && !PresentParams.Windowed &&
        D3D->CheckDeviceMultiSampleType(0,D3DDeviceCaps.DeviceType,
            GD3DFormats[CPixelFormat(PresentParams.BackBufferFormat).FormatIndex].D3DFormat,0,
            static_cast<D3DMULTISAMPLE_TYPE>(parameters.MultisampleType),0)==0)
        MultisampleType=parameters.MultisampleType;
    else
        MultisampleType=SURFACE_MULTISAMPLE_NONE;
    PresentParams.MultiSampleType=static_cast<D3DMULTISAMPLE_TYPE>(MultisampleType);
    if (D3D->CreateDevice(D3DDeviceCaps.AdapterOrdinal,D3DDeviceCaps.DeviceType,WindowHandle,flags,&PresentParams,&D3DDevice)<0)
        return CREATE_DEVICE_FAILED;
    D3DDISPLAYMODE mode;
    if (D3D->GetAdapterDisplayMode(0,&mode)<0)
        throw CGenericException();
    RefreshRate=mode.RefreshRate;
    PresentationIntervalTime=static_cast<float>(PresentationInterval.VSyncCount)/static_cast<float>(RefreshRate);
    VsyncCallbackTimer->SetDevice(D3DDevice);
    IDirect3DSurface9* surface;
    if (D3DDevice->GetBackBuffer(0,0,0,&surface)<0)
        return CREATE_DEVICE_FAILED;
    CSurface& backBuffer=BackBuffer;
    backBuffer.InitialiseFromPreallocatedSurface(surface,false);
    C2DExtentsI dimensions=backBuffer.GetDimensions();
    BackBufferDimensionsStorage_[0]=dimensions.X;
    BackBufferDimensionsStorage_[1]=dimensions.Y;
    BackBufferFormatStorage_=backBuffer.GetPixelFormat().FormatIndex;
    IDirect3DSurface9* depthSurface;
    if (D3DDevice->GetDepthStencilSurface(&depthSurface)<0)
        return CREATE_DEVICE_FAILED;
    CSurface& depthBuffer=DepthBuffer;
    depthBuffer.InitialiseFromPreallocatedSurface(depthSurface,false);
    DepthBufferFormatStorage_=depthBuffer.GetPixelFormat().FormatIndex;
    RenderTargetSurface=backBuffer;
    RenderTargetDepthBuffer=depthBuffer;
    RenderTargetDimensionsStorage_[0]=BackBufferDimensionsStorage_[0];
    RenderTargetDimensionsStorage_[1]=BackBufferDimensionsStorage_[1];
    if (!SetupDepthAndBackBufferTextures())
        return CREATE_DEVICE_FAILED;
    C2DBoxI viewport;
    viewport.left=viewport.top=0;
    viewport.right=RenderTargetDimensionsStorage_[0];
    viewport.bottom=RenderTargetDimensionsStorage_[1];
    SetIntegerViewportEndpoint(viewport);
    return CREATE_DEVICE_SUCCESS;
}
