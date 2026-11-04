#include "fable_render_state_manager.h"
#include "fable_surface.h"
#include "fable_system_font.h"
void CRenderManagerCore::Init(IDirect3DDevice9* device)
{
    CRenderStateManager::Init(device);
    PD3DDevice=device;
    SystemFont.Initialise();
    TexturesEnabled=true;
    FixedFunctionPipelineEnabled=true;
    HighestMipmapToRender=0;
    PSelectedIndexBuffer=0;
    PD3DDevice->GetDeviceCaps(&D3DDeviceCaps);
    TextureStagesInUse=0;
    Rendering=false;
    ScreenPixelFormat.FormatIndex=DisplayManager->GetBackBuffer().GetPixelFormat().FormatIndex;
    XBoxShaderConstantsEnabled=false;
}
void CRenderManagerCore::Uninit()
{
    CSystemFont* font=&SystemFont;
    if(font->IsValid()) font->Uninitialise();
}
