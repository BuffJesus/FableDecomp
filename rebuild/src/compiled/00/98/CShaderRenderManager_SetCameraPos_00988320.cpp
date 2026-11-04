#include "fable_shader_render_manager.h"
void CShaderRenderManager::SetCameraPos(const C3DVector& position)
{
    CameraPos=position;
    updateFlags3D8|=8;
}
