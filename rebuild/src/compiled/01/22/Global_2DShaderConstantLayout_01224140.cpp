#include "fable_shader_constant_layout.h"
#include "fable_string.h"

// The compiler emits the retail CRT initializer and atexit destructor thunk.
CVertexShaderConstantLayoutEmpty Fable2DShaderConstantLayout_013ca8a8;

CCharString CVertexShaderConstantLayoutEmpty::PeekName()
{
    return CCharString("LayoutEmpty",-1);
}
