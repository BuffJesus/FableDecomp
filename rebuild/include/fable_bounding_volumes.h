#pragma once
#include "fable_2d_vector.h"
class CMatrix3x4;
// Donor PDB layouts and retail B24150/ADEBC0 field accesses.
class C3DBoundingSphere {
public:
    C3DBoundingSphere() : Center(0.0f,0.0f,0.0f),Radius(0.0f) {}
    // Ego_r inline accessors; the centre comes back as a copy (C3DMeshPartitionElement::WriteToFile 0x00AC27A0).
    const C3DVector GetCenter() const { return Center; }
    float GetRadius() const { return Radius; }
    C3DVector Center;
    float Radius;
};
class C3DBoundingBox {
public:
    void CalculateMatrix(CMatrix3x4& matrix) const;
    // Ego_r inline accessors.
    const C3DVector& PeekMinCoord() const { return MinCoord; }
    const C3DVector& PeekMaxCoord() const { return MaxCoord; }
    C3DVector MinCoord,MaxCoord;
};
class CPlane {
public:
    C3DVector Normal;
    float Distance;
};
FABLE_STATIC_ASSERT(sizeof(C3DBoundingSphere)==16);
FABLE_STATIC_ASSERT(sizeof(C3DBoundingBox)==24);
FABLE_STATIC_ASSERT(sizeof(CPlane)==16);
