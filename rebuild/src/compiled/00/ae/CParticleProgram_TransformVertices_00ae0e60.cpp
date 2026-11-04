// CParticleProgram::TransformVertices (private; retail 0x00AE0E60, Ego_r 0x00BECCA0): skin count vertices. The output is
// cleared; then for every group with vertex blends, the blended input vertices are gathered into the group scratch
// buffer, transformed by the group's bone matrix (w = 1), and each added to its output vertex scaled by its blend weight.
// The accumulation has the engine's SSE / x87 switch on GEnableSIMD (the SSE path is reproduced with intrinsics).
#include "fable_particle_program.h"
#include "fable_vector_math.h"
#include "fable_matrix4x4_simd.h"

void CParticleProgram::TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count,
                                         const CMatrix4x4* matrices, const CArray<C3DGroup2>& groups) const
{
    NVectorMath::GFClear4DVectors(out, count, C4DVector(0.0f, 0.0f, 0.0f, 0.0f));
    for (unsigned long g = 0; g < groups.size(); ++g)
    {
        const C3DGroup2& group = groups[g];
        unsigned long blendCount = group.GetVertexBlendCount();
        if (blendCount)
        {
            const CMatrix4x4& matrix = matrices[group.GetBoneIndex()];
            const C3DVertexBlend2* blends = group.PeekVertexBlends();
            C4DVector* buffer = GetGroupVectorBuffer(blendCount);
            for (unsigned long i = 0; i < blendCount; ++i)
                buffer[i] = in[blends[i].GetVertexIndex()];
            matrix.AlignedTransformTransposedAssumeW(buffer, blendCount, 1.0f);
            for (unsigned long i = 0; i < blendCount; ++i)
            {
                C4DVector& target = out[blends[i].GetVertexIndex()];
                if (GEnableSIMD)
                {
                    __declspec(align(16)) float weight = blends[i].GetBlendValue();
                    __m128 scaled = _mm_load_ps(&buffer[i].X);
                    __m128 sum = _mm_load_ps(&target.X);
                    __m128 w = _mm_load_ps(&weight);
                    w = _mm_shuffle_ps(w, w, 0);
                    scaled = _mm_mul_ps(scaled, w);
                    sum = _mm_add_ps(sum, scaled);
                    _mm_store_ps(&target.X, sum);
                }
                else
                    target += buffer[i] * blends[i].GetBlendValue();
            }
        }
    }
}
