#include "fable_shader_resource.h"
#include "fable_counted_pointer.h"
void CShaderResource::Initialise(CShaderRenderManager& manager) { ShaderRenderManager=&manager; }
void CShaderResource::SetVertexShaderDeclaration(const D3DVERTEXELEMENT9* declaration) { VertexShaderDeclaration=const_cast<D3DVERTEXELEMENT9*>(declaration); }
void CShaderResource::SetVertexFormat(EVertexType format) { VertexFormat=format; }
const CShaderDataBank* CShaderResource::PeekShaderBank() const { return ShaderBank; }
template<> CCountedPointer<CShaderResource>& CCountedPointer<CShaderResource>::operator=(const CCountedPointer<CShaderResource>& other)
{
    CShaderResource* data=other.Data;
    CCPPointerInfo* info=other.Info;
    if(Info!=info) {
        Release();
        Data=data;
        Info=info;
        if(info) ++info->RefCount;
    }
    return *this;
}
