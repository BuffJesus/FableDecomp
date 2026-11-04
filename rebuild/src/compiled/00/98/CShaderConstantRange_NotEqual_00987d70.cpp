#include "fable_shader_constant_layout.h"

bool CShaderConstantRange::operator!=(const CShaderConstantRange& other) const
{
    return Offset!=other.Offset || Count!=other.Count;
}
