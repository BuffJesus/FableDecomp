#pragma once
#include <xmmintrin.h>
#include "fable_matrix4x4.h"

// Ego_r.pdb GEnableSIMD (retail 0x013D2880, .bss): start-up CPU detection enables the SSE paths.
extern bool GEnableSIMD;

// Ego_r.pdb CMatrix4x4::AlignedSetMultiply (FableWin 02E8E180), inline in retail: this = m1 * m2 for
// 16-byte aligned matrices. The engine's SSE path is an inline-asm block (m2's rows held in xmm0-3,
// each m1 element broadcast); these intrinsics reproduce its results, not its bytes. The scalar path
// keeps the engine's element loop, whose expression shape decides how VC7.1 associates the sums.
__forceinline void CMatrix4x4::AlignedSetMultiply(const CMatrix4x4& m1,const CMatrix4x4& m2)
{
    if (GEnableSIMD)
    {
        const __m128 r0 = _mm_load_ps(m2.Data[0]);
        const __m128 r1 = _mm_load_ps(m2.Data[1]);
        const __m128 r2 = _mm_load_ps(m2.Data[2]);
        const __m128 r3 = _mm_load_ps(m2.Data[3]);
#define FABLE_ALIGNED_MULTIPLY_ROW(row_)                                  \
        {                                                                 \
            __m128 x = _mm_load_ps(m1.Data[row_]);                        \
            __m128 y = x;                                                 \
            __m128 z = x;                                                 \
            __m128 w = x;                                                 \
            x = _mm_shuffle_ps(x, x, _MM_SHUFFLE(0, 0, 0, 0));            \
            y = _mm_shuffle_ps(y, y, _MM_SHUFFLE(1, 1, 1, 1));            \
            z = _mm_shuffle_ps(z, z, _MM_SHUFFLE(2, 2, 2, 2));            \
            w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));            \
            x = _mm_mul_ps(x, r0);                                        \
            y = _mm_mul_ps(y, r1);                                        \
            z = _mm_mul_ps(z, r2);                                        \
            w = _mm_mul_ps(w, r3);                                        \
            _mm_store_ps(Data[row_], _mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w))); \
        }
        FABLE_ALIGNED_MULTIPLY_ROW(0)
        FABLE_ALIGNED_MULTIPLY_ROW(1)
        FABLE_ALIGNED_MULTIPLY_ROW(2)
        FABLE_ALIGNED_MULTIPLY_ROW(3)
#undef FABLE_ALIGNED_MULTIPLY_ROW
    }
    else
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                Data[i][j] = m1(i, 0) * m2(0, j) + m1(i, 1) * m2(1, j) + m1(i, 2) * m2(2, j) + m1(i, 3) * m2(3, j);
    }
}

// Ego_r.pdb CMatrix4x4::AlignedSetMultiplyTransposed (FableWin 02E8E150): this = b * a.
__forceinline void CMatrix4x4::AlignedSetMultiplyTransposed(const CMatrix4x4& a,const CMatrix4x4& b)
{
    AlignedSetMultiply(b, a);
}
