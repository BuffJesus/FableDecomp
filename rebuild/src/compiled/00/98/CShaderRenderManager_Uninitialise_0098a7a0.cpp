#include "fable_shader_render_manager.h"
#include "fable_shader_resource.h"
void CShaderRenderManager::Uninitialise() {
    for(unsigned long i=0;i<Resources().size();++i)
        Resources()[i]->Uninitialise();
    D3DDevice=0;
}
