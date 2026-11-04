#include "fable_shader_render_manager.h"

void CShaderRenderManager::UnregisterVertexShaderConstantLayouts()
{
    CurrentVSConstantLayoutIndex=-1;
    const CVertexShaderConstantLayout* empty=0;
    ConstantLayouts().resize(1,empty);
    const CVertexShaderConstantLayout* nullLayout=&NullConstantLayout;
    ConstantLayouts()[0]=nullLayout;
    CurrentVSConstantLayout=nullLayout;
}
