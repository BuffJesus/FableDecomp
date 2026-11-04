#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include "fable_exception.h"
#include <string.h>

CDisplayManager::ECreateDeviceSuccess CDisplayManager::ResetDevice(const CCreateDeviceParameters& parameters)
{
    LastDeviceCreationParameters=parameters;
    PreDeviceReset();
    BackBuffer.Uninitialise();
    DepthBuffer.Uninitialise();
    RenderTargetSurface.Uninitialise();
    RenderTargetDepthBuffer.Uninitialise();
    BackBufferCopyTexture.Uninitialise();
    CurrentMode=GetClosestMode(parameters.BackBufferDimensions,parameters.BackBufferBitDepth,parameters.ExclusiveMode);
    CurrentScreenIndex=0;
    NoBackBuffers=parameters.BackBufferCount;
    PresentationInterval=parameters.PresentationInterval;
    memset(&PresentParams,0,sizeof(PresentParams));
    PresentParams.BackBufferWidth=CurrentMode.Width;
    PresentParams.BackBufferHeight=CurrentMode.Height;
    PresentParams.BackBufferFormat=CurrentMode.Format;
    PresentParams.Windowed=!parameters.ExclusiveMode;
    if (parameters.MultisampleType!=SURFACE_MULTISAMPLE_NONE && !PresentParams.Windowed &&
        D3D->CheckDeviceMultiSampleType(0,D3DDeviceCaps.DeviceType,
            GD3DFormats[CPixelFormat(CurrentMode.Format).FormatIndex].D3DFormat,0,
            static_cast<D3DMULTISAMPLE_TYPE>(parameters.MultisampleType),0)==0)
        MultisampleType=parameters.MultisampleType;
    else
        MultisampleType=SURFACE_MULTISAMPLE_NONE;
    PresentParams.MultiSampleType=static_cast<D3DMULTISAMPLE_TYPE>(MultisampleType);
    PresentParams.SwapEffect=D3DSWAPEFFECT_DISCARD;
    PresentParams.EnableAutoDepthStencil=1;
    PresentParams.AutoDepthStencilFormat=GD3DFormats[GetClosestDepthBufferFormat(CurrentMode,parameters.ZBufferBitDepth).FormatIndex].D3DFormat;
    PresentParams.hDeviceWindow=WindowHandle;
    PresentParams.BackBufferCount=NoBackBuffers;
    if (parameters.ExclusiveMode)
    {
        PresentParams.FullScreen_RefreshRateInHz=parameters.RefreshRate;
        PresentParams.PresentationInterval=CalculateD3DPresentationInterval(PresentationInterval);
    }
    else
    {
        PresentParams.FullScreen_RefreshRateInHz=0;
        PresentParams.PresentationInterval=0;
    }
    HRESULT result=D3DDevice->Reset(&PresentParams);
    if (result<0)
    {
        if (result==D3DERR_DEVICELOST)
        {
            DeviceLostFlag=true;
            return CREATE_DEVICE_DEVICE_LOST;
        }
        DeviceLostFlag=false;
        return CREATE_DEVICE_FAILED;
    }
    DeviceLostFlag=false;
    D3DDISPLAYMODE mode;
    if (D3D->GetAdapterDisplayMode(0,&mode)<0)
        throw CGenericException();
    RefreshRate=mode.RefreshRate;
    PresentationIntervalTime=static_cast<float>(PresentationInterval.VSyncCount)/static_cast<float>(RefreshRate);
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
    ExclusiveMode=parameters.ExclusiveMode;
    if (!SetupDepthAndBackBufferTextures())
        return CREATE_DEVICE_FAILED;
    return PostDeviceReset()?CREATE_DEVICE_SUCCESS:CREATE_DEVICE_FAILED;
}
