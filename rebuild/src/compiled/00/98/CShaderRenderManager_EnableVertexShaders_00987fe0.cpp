#include "fable_shader_render_manager.h"
#include "fable_display_viewport.h"

void CShaderRenderManager::EnableVertexShaders()
{
    VertexShadersEnabled = true;
    CurrentVertexShader = 0;
    SWLightingEnable = false;
    RenderManager->FixedFunctionPipelineEnabled = false;
    RenderManager->EnableXBoxShaderConstants(true);
    updateFlags3D8 = 0xFFFFFFFF;
}
