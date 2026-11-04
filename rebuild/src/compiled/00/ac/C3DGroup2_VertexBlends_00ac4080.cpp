// C3DGroup2's vertex-blend accessors (retail 0x00AC4080 / 0x00AC40A0): the blends as a C array.
#include "fable_3d_group2.h"

// 0x00AC4080
C3DVertexBlend2* C3DGroup2::GetVertexBlends()
{
    return VertexBlends.GetAsCArray();
}

// 0x00AC40A0
const C3DVertexBlend2* C3DGroup2::PeekVertexBlends() const
{
    return VertexBlends.GetAsCArray();
}
