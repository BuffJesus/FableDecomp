#include "fable_shader_render_manager.h"
#include "fable_display_viewport.h"

void CShaderRenderManager::DisableVertexShaders()
{
    VertexShadersEnabled = false;
    CurrentVertexShader = 0;
    RenderManager->EnableXBoxShaderConstants(false);
    RenderManager->FixedFunctionPipelineEnabled = true;
}
