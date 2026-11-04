#pragma once
#include "rebuild_abi.h"
#include "engine/CMatrix3x4.h"

// Ego_r.pdb names and 64-byte layout. Alignment of containing native storage
// is specified by its owner, not imposed on this partial value interface.
class C4DVector;
class CMatrix4x4
{
public:
    // Ego_r inline constructors: empty default, and from a 3x4 matrix stored transposed (rows of the 3x4 become
    // columns; last row 0 0 0 1) - element order read from Ego_r CParticleSimulation::ApplyDragging 0x00BEAFB0. Retail
    // ApplyDragging 0x00ADF300 expands the conversion; VC7.1 keeps a plain inline out of line there (GOTCHAS: __forceinline).
    CMatrix4x4() {}
    __forceinline CMatrix4x4(const CMatrix3x4& m)
    {
        E11 = m.E11; E12 = m.E21; E13 = m.E31; E14 = m.E41;
        E21 = m.E12; E22 = m.E22; E23 = m.E32; E24 = m.E42;
        E31 = m.E13; E32 = m.E23; E33 = m.E33; E34 = m.E43;
        E41 = 0.0f; E42 = 0.0f; E43 = 0.0f; E44 = 1.0f;
    }
    void Transpose(); // retail 00A5CAB0; aligned local copy, no owner alignment change
    // Ego_r element read (FableWin 02E8E450).
    float const operator()(int row,int column) const { return Data[row][column]; }
    // Ego_r inlines; bodies (SSE switch on GEnableSIMD) in fable_matrix4x4_simd.h.
    __forceinline void AlignedSetMultiply(const CMatrix4x4& m1,const CMatrix4x4& m2);
    __forceinline void AlignedSetMultiplyTransposed(const CMatrix4x4& a,const CMatrix4x4& b);
    // Ego_r ?AlignedSetInterpolate@CMatrix4x4@@QAEXABV1@0M@Z (retail 0x00A5CEE0) and its inline in-place overload.
    void AlignedSetInterpolate(const CMatrix4x4& a,const CMatrix4x4& b,float t);
    void AlignedSetInterpolate(const CMatrix4x4& b,float t) { AlignedSetInterpolate(*this,b,t); }
    // Ego_r ?AlignedTransformTransposedAssumeW@CMatrix4x4@@QBEXPBVC4DVector@@PAV2@KM@Z (retail 0x00A5CB90) and its
    // inline in-place overload.
    void AlignedTransformTransposedAssumeW(const C4DVector* in,C4DVector* out,unsigned long count,float w) const;
    void AlignedTransformTransposedAssumeW(C4DVector* vectors,unsigned long count,float w) const
    {
        AlignedTransformTransposedAssumeW(vectors,vectors,count,w);
    }
    // Ego_r CMatrix4x4::Identity (non-const per Ego_r.pdb; retail 0x013D2890, dynamically initialised).
    static CMatrix4x4 Identity;
    union
    {
        struct
        {
            float E11,E12,E13,E14;
            float E21,E22,E23,E24;
            float E31,E32,E33,E34;
            float E41,E42,E43,E44;
        };
        float Data[4][4];
    };
};
FABLE_STATIC_ASSERT(sizeof(CMatrix4x4)==64);
