#include "fable_shader_render_manager.h"
#include "fable_display_viewport.h"

void CShaderRenderManager::EnablePixelShaders()
{
    if (RenderManager->DisplayManager->GetPixelShaderVersion() >= 1000)
        PixelShadersEnabled = true;
}
