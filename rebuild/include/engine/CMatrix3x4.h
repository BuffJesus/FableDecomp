#ifndef FABLE_ENGINE_CMATRIX3X4_H
#define FABLE_ENGINE_CMATRIX3X4_H

#include <stddef.h>
#include "fable_2d_vector.h"

#pragma pack(push, 1)
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;

    // Ego_r ?SetIdentity@CMatrix3x4@@QAEXXZ; inline in retail
    // (C3DMeshFileTransformationChunk::CreateNewInstance 0x00AB9CE0).
    void SetIdentity()
    {
        E12 = E13 = E21 = E23 = E31 = E32 = E41 = E42 = E43 = 0.0f;
        E11 = E22 = E33 = 1.0f;
    }

    C3DVector GetTranslation() const
    {
        return C3DVector(E41,E42,E43);
    }
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CMatrix3x4) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CMatrix3x4, E41) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CMatrix3x4, E42) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CMatrix3x4, E43) == 0x2c);

#endif // FABLE_ENGINE_CMATRIX3X4_H
