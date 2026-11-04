// Retail 009C0E50; FableWin public display initialization.
#include "fable_display_viewport.h"
#include "fable_system.h"
#include "fable_shader_render_manager.h"
#include "fable_exception.h"
extern "C" IDirect3D9* FABLE_STDCALL Direct3DCreate9(unsigned int);
void CDisplayManager::Init(const CDisplayManagerInit& init,bool exclusiveMode)
{
    DeviceLostFlag=false;
    ExclusiveMode=exclusiveMode;
    UsingRenderManager=init.UseRenderManager;
    UsingShaderRenderManager=init.UseShaderRenderManager && init.UseRenderManager;
    MultisampleType=SURFACE_MULTISAMPLE_NONE;
    CanBeginRender=false;
    WindowHandle=static_cast<HWND__*>(init.WindowHandle);
    D3D=Direct3DCreate9(32);
    VsyncCallbackTimer.reset(new CVsyncCallbackTimer);
    if(D3D->GetDeviceCaps(0,D3DDEVTYPE_HAL,&D3DDeviceCaps)<0)
        D3D->GetDeviceCaps(0,D3DDEVTYPE_REF,&D3DDeviceCaps);
    CCreateDeviceParameters parameters;
    long width=init.Width, height=init.Height, depth=init.Depth;
    if(width<32) width=32;
    if(height<32) height=32;
    parameters.WinHandle=WindowHandle;
    parameters.BackBufferDimensions.X=width;
    parameters.BackBufferDimensions.Y=height;
    parameters.BackBufferBitDepth=depth;
    parameters.ZBufferBitDepth=init.ZBufferDepth;
    parameters.BackBufferCount=init.NoBackBuffers;
    parameters.RefreshRate=init.RefreshRate;
    parameters.ExclusiveMode=exclusiveMode;
    parameters.PresentationInterval.PresentImmediate=init.PresentImmediate;
    parameters.PresentationInterval.PresentImmediateIfSyncMissed=init.PresentImmediatelyIfMissedVSync;
    parameters.PresentationInterval.VSyncCount=init.PresentationIntevalVSyncCount;
    parameters.MultisampleType=static_cast<ESurfaceMultisampleType>(init.MultisampleType);
    if(CreateDevice(parameters)!=CREATE_DEVICE_SUCCESS) throw CGenericException();
    ResizeWindowToBackBufferSize();
    RenderManagerInitFlags=init.RenderManagerInitFlags;
    if(UsingRenderManager) {
        ShaderResourcePath=init.ShaderResourcePath;
        PRenderManager.reset(new CRenderManager(*this,ShaderResourcePath,init.RenderManagerInitFlags));
        PRenderManager->Init(D3DDevice);
        if(UsingShaderRenderManager)
            FableShaderRenderManager_013bc470.Initialise(PRenderManager.get(),D3DDevice);
    }
    InitVirtualPixels();
}
