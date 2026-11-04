#pragma once
#include "rebuild_abi.h"
#include "fable_2d_vector.h"

// Ego_r.pdb C4DVector, used by shader constant arrays.
class C4DVector
{
public:
    C4DVector() {}
    C4DVector(float x,float y,float z,float w):X(x),Y(y),Z(z),W(w) {}
    // Ego_r inline members (used by CParticleSimulation::ApplyForces 0x00ADF760).
    C4DVector(const C3DVector& v,float w):X(v.X),Y(v.Y),Z(v.Z),W(w) {}
    const C4DVector operator+(const C4DVector& o) const { return C4DVector(X+o.X,Y+o.Y,Z+o.Z,W+o.W); }
    const C4DVector operator-(const C4DVector& o) const { return C4DVector(X-o.X,Y-o.Y,Z-o.Z,W-o.W); }
    const C4DVector operator*(float s) const { return C4DVector(X*s,Y*s,Z*s,W*s); }
    const C4DVector& operator+=(const C4DVector& o) { X+=o.X; Y+=o.Y; Z+=o.Z; W+=o.W; return *this; }
    C3DVector ToC3DVector() const { return C3DVector(X,Y,Z); }
    float X, Y, Z, W;
};
FABLE_STATIC_ASSERT(sizeof(C4DVector)==16);
