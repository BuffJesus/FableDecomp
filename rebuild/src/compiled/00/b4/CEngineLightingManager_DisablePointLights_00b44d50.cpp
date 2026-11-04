#include "engine/CEngineLightingManager.h"
#include "fable_engine_internal.h"
#include "fable_shader_render_manager.h"
void CEngineLightingManager::DisablePointLights()
{
    for(long i=0;i<ShaderPointLightsEnabledCount;++i)
        NEngineInternal::GPShaderRenderManager->ResetLight(i+1);
    ShaderPointLightsEnabledCount=0;
}
