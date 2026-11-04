#include "fable_shader_render_manager.h"
void CShaderRenderManager::ResetLight(unsigned long index)
{
    if(index>=LightCount)return;
    bool* enabled=&Lights[index].IsEnabled;
    if(!*enabled)return;
    *enabled=false;
    Lights[index].UpdateFlag=true;
    updateFlags3D8|=0x700;
}
