#include "fable_shader_resource.h"

void CShaderResource::SetVertexShaderInput(const CVertexShaderInput& input)
{
    VertexShaderInput = input;
    // Native PeekD3DDeclaration uses the array object as its empty fallback.
    if (VertexShaderInput.D3DDeclaration.size() > 0)
    {
        VertexShaderDeclaration = VertexShaderInput.D3DDeclaration.begin();
        return;
    }
    VertexShaderDeclaration = reinterpret_cast<D3DVERTEXELEMENT9*>(&VertexShaderInput.D3DDeclaration);
}
