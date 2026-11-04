#include "fable_shader_data_bank.h"
CShaderBankEntry* CShaderDataBank::GetVertexShader(unsigned long index) const
{
    return Shaders[index];
}
