#include "fable_shader_resource.h"
#include "fable_shader_data_bank.h"
CVertexShader CShaderResource::GetVertexShader(const CCharString& name) const
{
 return GetVertexShader(ShaderBank->FindIndexBySymbol(name));
}
