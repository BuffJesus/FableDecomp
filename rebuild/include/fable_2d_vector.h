#pragma once

// C2DVector / C3DVector (Ego_r.pdb layouts: X,Y / X,Y,Z floats, 8 / 12 bytes).
// Only the constructors the reconstructed callers use are modelled.

#include "rebuild_abi.h"
#include "fable_vector_magnitude.h"   // GFSqr, GFFastOneOverSqrt

class C2DVector
{
public:
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    float GetSquaredMagnitude() const {return X*X+Y*Y;}
    // Ego_r ?Normalise@C2DVector@@QAEAAV1@XZ (retail 0x00A14540): scale to unit length (GFFastOneOverSqrt).
    C2DVector& Normalise();
    bool IsNonZero() const;

    float X;
    float Y;
};

class C3DVector
{
public:
    C3DVector() {}
    // FableWin ??0C3DVector@@QAE@MMM@Z; inline in retail
    // (CNewFrontendGameComponent::InitialiseEngine 0x0042E204).
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    // User-declared member-wise copy (Ego_r.pdb lists ??0C3DVector@@QAE@ABV0@@Z). Retail copy-constructs vectors one
    // float at a time through one register (CParticleSimulation::GetBoundingBox 0x00ADF730 is exact only with this),
    // and it makes C3DVector non-POD to STLport; landed stand-ins at 0x0088F9B0 / 0x007BEDF0 / 0x008CFE30 had to
    // declare the same constructor locally to match.
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}

    // FableWin ??DC3DVector@@QBE?BV0@M@Z; inline in retail
    // (CParticleEnvironmentInteraction::GetWindVelocity 0x0049BE10).
    const C3DVector operator*(float scale) const
    {
        return C3DVector(X * scale, Y * scale, Z * scale);
    }

    const C3DVector& operator+=(const C3DVector& other)
    {
        X += other.X; Y += other.Y; Z += other.Z;
        return *this;
    }
    const C3DVector operator-(const C3DVector& other) const { return C3DVector(X-other.X,Y-other.Y,Z-other.Z); }
    const C3DVector operator+(const C3DVector& other) const { return C3DVector(X+other.X,Y+other.Y,Z+other.Z); }
    // Ego_r inline members (COptimisedPrimitive::GetFaceNormal 0x00AC7CB0 expands them).
    float GetSquaredMagnitude() const { return GFSqr(X) + GFSqr(Y) + GFSqr(Z); }
    float GetAccurateMagnitude() const { return GFRoot(GetSquaredMagnitude()); }
    const float Dot(const C3DVector& other) const { return X * other.X + Y * other.Y + Z * other.Z; }
    const C3DVector& operator-=(const C3DVector& other)
    {
        X -= other.X; Y -= other.Y; Z -= other.Z;
        return *this;
    }
    const C3DVector& operator*=(float scale)
    {
        X *= scale; Y *= scale; Z *= scale;
        return *this;
    }
    // Cross product.
    const C3DVector operator^(const C3DVector& other) const
    {
        return C3DVector(Y * other.Z - Z * other.Y, Z * other.X - X * other.Z, X * other.Y - Y * other.X);
    }
    float GetOneOnMagnitude() const { return GFFastOneOverSqrt(GetSquaredMagnitude()); }
    bool IsNonZero() const;
    // Unit length through the fast reciprocal root.
    __forceinline C3DVector& Normalise()
    {
        *this *= GFFastOneOverSqrt(GetSquaredMagnitude());
        return *this;
    }
    C3DVector& AccurateNormalise(); // retail 00A14440
    float GetAngleXY() const;
    float GetAngleYZ() const;
    const C3DVector& InitialiseFromAngles(float angle_xy, float angle_yz, float distance);

    float X;
    float Y;
    float Z;
};

// ?DELTA_NOTIONAL_ZERO@@3MB (FableWin.pdb; value 0.0001f). Retail 0x0129BA3C.
extern const float DELTA_NOTIONAL_ZERO;
inline bool C2DVector::IsNonZero() const {return GetSquaredMagnitude()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
inline bool C3DVector::IsNonZero() const {return GetSquaredMagnitude()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}

FABLE_STATIC_ASSERT(sizeof(C2DVector) == 0x08);
FABLE_STATIC_ASSERT(sizeof(C3DVector) == 0x0C);
