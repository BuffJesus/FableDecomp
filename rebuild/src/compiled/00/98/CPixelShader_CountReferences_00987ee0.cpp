#include "fable_pixel_shader.h"

unsigned long CPixelShader::CountReferences()
{
    return Data->ReferenceCount;
}
