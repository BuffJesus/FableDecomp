#ifndef FABLE_ENGINE_CMATRIX3X4_H
#define FABLE_ENGINE_CMATRIX3X4_H

#include <stddef.h>
#include "C3DVector.h"

#pragma pack(push, 1)
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;

    C3DVector GetTranslation() const
    {
        C3DVector translation = { E41, E42, E43 };
        return translation;
    }
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CMatrix3x4) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CMatrix3x4, E41) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CMatrix3x4, E42) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CMatrix3x4, E43) == 0x2c);

#endif // FABLE_ENGINE_CMATRIX3X4_H
