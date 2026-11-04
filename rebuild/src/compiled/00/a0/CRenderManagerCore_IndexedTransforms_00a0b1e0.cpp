// lib_render_manager_core.cpp, retail 0x00A0AAD0, 0x00A0B1E0, 0x00A0B280 and
// 0x00A0B3E0 (Ego_r CRenderManagerCore::SetD3DMatrix, SetWorldTransform(index),
// SetTextureTransform, SetPerspectiveTransform(16 floats)). The 3x4 forms are
// expanded to a 4x4 with an (0,0,0,1) last column, then passed to SetTransform.
#include "fable_display_viewport.h"
#include "fable_matrix4x4.h"

// The same expansion CRenderManagerCore_MatrixSetters_00a0b0a0.cpp inlines.
// Here the last column is written as floats: retail stores immediate zeros
// after pushing the transform state, and the integer form lets VC7.1 share
// a zeroed register instead.
static __forceinline void expandMatrix(CMatrix4x4& destination, const CMatrix3x4& source)
{
    unsigned long* out = reinterpret_cast<unsigned long*>(&destination);
    const unsigned long* in = reinterpret_cast<const unsigned long*>(&source);
    out[0] = in[0]; out[1] = in[1]; out[2] = in[2];
    out[4] = in[3]; out[5] = in[4]; out[6] = in[5];
    out[8] = in[6]; out[9] = in[7]; out[10] = in[8];
    out[12] = in[9]; out[13] = in[10]; out[14] = in[11];
    destination.E14 = 0.0f; destination.E24 = 0.0f; destination.E34 = 0.0f; destination.E44 = 1.0f;
}

// Retail 00A0B1E0: one of the indexed world matrices (D3DTS_WORLDMATRIX(index)).
void CRenderManagerCore::SetWorldTransform(unsigned long index, const CMatrix3x4& transform)
{
    CMatrix4x4 matrix;
    expandMatrix(matrix, transform);
    PD3DDevice->SetTransform(256 + index, reinterpret_cast<const D3DMATRIX*>(&matrix));
}

// The stage -> D3DTS_TEXTUREn table (retail .data 0x0139703C holds 16..23;
// its Ego_r name was not recovered).
extern int g_FableTextureTransformStates_0139703C[8];

// Retail 00A0B280.
void CRenderManagerCore::SetTextureTransform(unsigned long stage, const CMatrix3x4& transform)
{
    CMatrix4x4 matrix;
    expandMatrix(matrix, transform);
    PD3DDevice->SetTransform(g_FableTextureTransformStates_0139703C[stage], reinterpret_cast<const D3DMATRIX*>(&matrix));
}

// Retail 00A0AAD0.
void CRenderManagerCore::SetD3DMatrix(D3DMATRIX& matrix, float e11, float e12, float e13, float e14,
    float e21, float e22, float e23, float e24, float e31, float e32, float e33, float e34,
    float e41, float e42, float e43, float e44)
{
    matrix._11 = e11; matrix._12 = e12; matrix._13 = e13; matrix._14 = e14;
    matrix._21 = e21; matrix._22 = e22; matrix._23 = e23; matrix._24 = e24;
    matrix._31 = e31; matrix._32 = e32; matrix._33 = e33; matrix._34 = e34;
    matrix._41 = e41; matrix._42 = e42; matrix._43 = e43; matrix._44 = e44;
}

// Retail 00A0B3E0: an explicit projection matrix (SetD3DMatrix inlined).
void CRenderManagerCore::SetPerspectiveTransform(float e11, float e12, float e13, float e14,
    float e21, float e22, float e23, float e24, float e31, float e32, float e33, float e34,
    float e41, float e42, float e43, float e44)
{
    D3DMATRIX matrix;
    SetD3DMatrix(matrix, e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44);
    PD3DDevice->SetTransform(3, &matrix); // D3DTS_PROJECTION
}
