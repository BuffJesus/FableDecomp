#pragma once
#include "rebuild_abi.h"

// Ego_r.pdb C2DExtentsF; returned by CFontBank's retail virtual slot 1.
class C2DExtentsF
{
public:
    C2DExtentsF() {}
    C2DExtentsF(float x, float y) : X(x), Y(y) {}
    C2DExtentsF(const C2DExtentsF& other) : X(other.X), Y(other.Y) {}
    float X;
    float Y;
};
FABLE_STATIC_ASSERT(sizeof(C2DExtentsF) == 8);
