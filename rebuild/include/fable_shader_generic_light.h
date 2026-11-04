#pragma once
#include "fable_4d_vector.h"

// Ego_r.pdb CShaderGenericLight; the default initializes only Size.
class CShaderGenericLight
{
public:
    enum EParameterType
    {
        PARAMTYPE_COLOUR=0,PARAMTYPE_DIRECTION=1,
        PARAMTYPE_POSITION=2,PARAMTYPE_ATTENUATION=3
    };
    CShaderGenericLight():Size(0){}
    C4DVector Parameters[2];
    EParameterType Types[2];
    float Attenuation2,AttenuationBase;
    unsigned long Size;
};
FABLE_STATIC_ASSERT(sizeof(CShaderGenericLight)==0x34);
