#include "fable_shader_render_manager.h"

void CShaderRenderManager::OnPostViewportChanged()
{
    updateFlags3D8 |= 0x10000;
}
