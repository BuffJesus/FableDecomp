#pragma once
#include "rebuild_abi.h"
// Ego_r.pdb CVertexShaderConstant, the four-float reserved-preset value.
class CVertexShaderConstant
{
public:
    float X,Y,Z,W;
};
FABLE_STATIC_ASSERT(sizeof(CVertexShaderConstant)==16);
