#include "fable_shader_resource.h"
CShaderResource::CShaderResource() : ShaderBank(0), VertexShaderDeclaration(0), VertexFormat(static_cast<EVertexType>(0))
{
    ShaderRenderManager=0;
}
void CShaderResource::CloseShaderBank()
{
    for(unsigned long i=0;i<Shaders.size();++i) {
        if(Shaders[i]) {
            delete Shaders[i];
            Shaders[i]=0;
        }
    }
    ShaderBank=0;
}
void CShaderResource::Uninitialise()
{
    CloseShaderBank();
    ShaderRenderManager=0;
}

CShaderResource::~CShaderResource() { Uninitialise(); }
