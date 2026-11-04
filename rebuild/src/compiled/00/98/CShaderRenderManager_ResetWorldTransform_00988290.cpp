#include "fable_shader_render_manager.h"

void CShaderRenderManager::ResetWorldTransform()
{
    if(!WorldMatrixIsIdentity)
    {
        WorldMatrix.E12=0.0f;
        WorldMatrix.E13=0.0f;
        WorldMatrix.E14=0.0f;
        WorldMatrix.E21=0.0f;
        WorldMatrix.E23=0.0f;
        WorldMatrix.E24=0.0f;
        WorldMatrix.E31=0.0f;
        WorldMatrix.E32=0.0f;
        WorldMatrix.E34=0.0f;
        WorldMatrix.E41=0.0f;
        WorldMatrix.E42=0.0f;
        WorldMatrix.E43=0.0f;
        WorldMatrix.E11=1.0f;
        WorldMatrix.E22=1.0f;
        WorldMatrix.E33=1.0f;
        WorldMatrix.E44=1.0f;
        updateFlags3D8 |= 0xc990;
        WorldMatrixIsIdentity=true;
    }
}
