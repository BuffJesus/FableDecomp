#pragma once
#include "rebuild_abi.h"
class C2DBoxF
{
public:
    // Ego_r.pdb inline constructors; the four-coordinate form is inlined
    // by retail CDisplayManager::GetVVisibleTelevisionScreenArea (009BECE0).
    C2DBoxF() {}
    C2DBoxF(const C2DBoxF& other)
        : left(other.left), top(other.top), right(other.right), bottom(other.bottom) {}
    C2DBoxF(float x0, float y0, float x1, float y1)
        : left(x0), top(y0), right(x1), bottom(y1) {}
    // FableWin ?Initialise@C2DBoxF@@QAEXMMMM@Z; inline in retail
    // (CNewFrontendGameComponent::Draw 0x0042DF9E).
    void Initialise(float x0, float y0, float x1, float y1)
    {
        left = x0;
        top = y0;
        right = x1;
        bottom = y1;
    }

    float left;
    float top;
    float right;
    float bottom;
};

// Ego_r C2DCoordI: an integer point (X, Y).
class C2DCoordI
{
public:
    long X;
    long Y;
};

// Ego_r C3DCoordI: C2DCoordI plus Z (12 bytes).
class C3DVector;
class C3DCoordI : public C2DCoordI
{
public:
    // Ego_r ??YC3DCoordI@@QAEABV0@ABVC3DVector@@@Z, retail 0x00A13BE0.
    const C3DCoordI& operator+=(const C3DVector& offset);

    long Z;
};

class C2DBoxI
{
public:
public:
    // Original PDB unsigned coordinates; retain earlier signed aliases.
    union { unsigned int TLX; fable_i32 left; };
    union { unsigned int TLY; fable_i32 top; };
    union { unsigned int BRX; fable_i32 right; };
    union { unsigned int BRY; fable_i32 bottom; };
};

