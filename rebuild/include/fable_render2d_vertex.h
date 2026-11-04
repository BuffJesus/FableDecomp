#pragma once
#include "rebuild_abi.h"
// Ego_r.pdb vertex inheritance and field layout. No field has a default value.
struct CTVertexRHWColSpecTex1Base
{
    float X,Y,Z,RHW;
    unsigned long DiffuseColour,SpecularColour;
    float U1,V1;
};
// The reciprocal-W clamp for |Z| < 0.01 (retail .data 0x0139652C = 100.0f;
// Ego_r name not recovered).
extern float g_FableVertexReciprocalLimit_0139652C;
// Ego_r CGenericVertex members (inline in retail; the rest are not modelled
// yet). SetRHWFromZ follows the Ego_r donor body (0x01AE1FD0).
template<class T> class CGenericVertex : public T {
public:
    void SetZ(float value) { this->Z = value; }
    __forceinline void SetRHWFromZ()
    {
        if (this->Z >= 0.0f && this->Z < 0.01f) this->RHW = g_FableVertexReciprocalLimit_0139652C;
        else if (this->Z >= -0.01f && this->Z < 0.0f) this->RHW = -g_FableVertexReciprocalLimit_0139652C;
        else this->RHW = 1.0f / this->Z;
    }
};
class CTVertexRHWColSpecTex1 : public CGenericVertex<CTVertexRHWColSpecTex1Base> {};
FABLE_STATIC_ASSERT(sizeof(CTVertexRHWColSpecTex1)==32);
