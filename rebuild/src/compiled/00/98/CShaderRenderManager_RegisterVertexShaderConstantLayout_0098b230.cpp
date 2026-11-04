#include "fable_shader_render_manager.h"

void CShaderRenderManager::RegisterVertexShaderConstantLayout(long index,const CVertexShaderConstantLayout* layout)
{
    if(static_cast<unsigned long>(index)>=ConstantLayouts().size())
    {
        const CVertexShaderConstantLayout* empty=0;
        ConstantLayouts().resize(static_cast<unsigned long>(index+1),empty);
    }
    ConstantLayouts()[index]=layout;
}
