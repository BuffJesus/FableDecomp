#include "fable_shader_resource.h"
#include "fable_shader_data_bank.h"
CVertexShader CShaderResource::GetVertexShader(unsigned long index) const
{
    if (Shaders[index]) return *static_cast<CVertexShader*>(Shaders[index]);
    CShaderBankEntry* entry = ShaderBank->GetVertexShader(index);
    CVertexShader* shader = new CVertexShader(entry->ShaderCode, VertexShaderDeclaration,
        ShaderBank->GetEntrySymbol(index), CCharString(entry->VSConstantLayout));
    Shaders[index] = shader;
    return *shader;
}
