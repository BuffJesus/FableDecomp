// Behaviour fixture for CParticleProgram::TransformVertices (provider CParticleProgram_TransformVertices_00ae0e60.cpp),
// reached through its mangled name (private). Groups are raw C3DGroup2 storage (BoneIndex +0, STLport blend vector
// +4/+8/+12 of 8-byte {VertexIndex, BlendValue}). The fixture supplies GFClear4DVectors, PeekVertexBlends,
// GetGroupVectorBuffer, AlignedTransformTransposedAssumeW (it scales xyz by the matrix's E11 and records the call) and
// GEnableSIMD. Checked, with SIMD off and on: the output is cleared to zero over count vectors; a group without blends is
// skipped (no scratch buffer, no transform); every other group gathers its blended inputs into the scratch buffer in
// blend order, transforms them once in place (count, w = 1) by its own bone matrix, and adds each transformed vector
// times its weight into the output vertex it names (vertices named twice accumulate).
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"
#include "fable_vector_math.h"
#include "fable_matrix4x4_simd.h"
#include "fable_4d_vector.h"

bool GEnableSIMD;
static int g_fail, g_clears, g_buffers, g_transforms;
static unsigned long g_clearCount, g_bufferCounts[4], g_transformCounts[4]; static const void* g_transformMatrices[4];
static float g_transformW[4]; static const void* g_transformIn[4];
static __declspec(align(16)) float g_buffer[8][4];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void NVectorMath::GFClear4DVectors(C4DVector* v, unsigned long count, const C4DVector& value)
{
    ++g_clears; g_clearCount = count; for (unsigned long i = 0; i < count; ++i) v[i] = value;
}
const C3DVertexBlend2* C3DGroup2::PeekVertexBlends() const
{
    const C3DVertexBlend2* b; memcpy(&b, reinterpret_cast<const unsigned char*>(this) + 4, 4); return b;
}
C4DVector* FABLE_FASTCALL CParticleProgram::GetGroupVectorBuffer(unsigned long count)
{
    g_bufferCounts[g_buffers++ & 3] = count; return reinterpret_cast<C4DVector*>(g_buffer);
}
void CMatrix4x4::AlignedTransformTransposedAssumeW(const C4DVector* in, C4DVector* out, unsigned long count, float w) const
{
    int k = g_transforms++ & 3; g_transformCounts[k] = count; g_transformMatrices[k] = this; g_transformW[k] = w; g_transformIn[k] = in;
    CHECK(in == out);
    for (unsigned long i = 0; i < count; ++i) { out[i].X *= E11; out[i].Y *= E11; out[i].Z *= E11; }
}

extern "C" void __fastcall FixTransform(const void* self, int, C4DVector* out, const C4DVector* in, unsigned long count,
                                        const CMatrix4x4* matrices, const void* groups);
#pragma comment(linker, "/alternatename:@FixTransform@28=?TransformVertices@CParticleProgram@@ABEXPAVC4DVector@@PBV2@KPBVCMatrix4x4@@ABV?$CArray@VC3DGroup2@@@@@Z")

struct Blend { unsigned long index; float weight; };
static void SetGroup(unsigned long* g, long bone, Blend* b, unsigned n)
{
    memcpy(g, &bone, 4); void* s = b; void* e = b + n; memcpy(g + 1, &s, 4); memcpy(g + 2, &e, 4); memcpy(g + 3, &e, 4);
}

static void Run(bool simd)
{
    GEnableSIMD = simd; g_clears = g_buffers = g_transforms = 0;
    static unsigned char program[0x140];
    static __declspec(align(16)) float in[4][4] = { { 1, 2, 3, 1 }, { 4, 5, 6, 1 }, { 7, 8, 9, 1 }, { 10, 11, 12, 1 } };
    static __declspec(align(16)) float out[5][4];
    static __declspec(align(16)) CMatrix4x4 matrices[3];
    memset(matrices, 0, sizeof(matrices)); matrices[0].E11 = 10.0f; matrices[2].E11 = 2.0f;
    memset(out, 0x7F, sizeof(out));
    static Blend b0[2] = { { 3, 0.5f }, { 1, 1.0f } };
    static Blend b2[2] = { { 1, 0.25f }, { 0, 2.0f } };
    static unsigned long groups[3][4];
    SetGroup(groups[0], 2, b0, 2); SetGroup(groups[1], 1, 0, 0); SetGroup(groups[2], 0, b2, 2);
    unsigned long list[3]; void* gs = groups; void* ge = groups + 3; memcpy(list, &gs, 4); memcpy(list + 1, &ge, 4); memcpy(list + 2, &ge, 4);

    FixTransform(program, 0, reinterpret_cast<C4DVector*>(out), reinterpret_cast<const C4DVector*>(in), 5, matrices, list);
    CHECK(g_clears == 1 && g_clearCount == 5);
    CHECK(g_buffers == 2 && g_bufferCounts[0] == 2 && g_bufferCounts[1] == 2);
    CHECK(g_transforms == 2 && g_transformMatrices[0] == &matrices[2] && g_transformMatrices[1] == &matrices[0]);
    CHECK(g_transformCounts[0] == 2 && g_transformW[0] == 1.0f && g_transformIn[0] == g_buffer);
    // group 0 (bone 2, x2): in3*2*0.5 -> out3 ; in1*2*1 -> out1.  group 2 (bone 0, x10): in1*10*0.25 -> out1 ; in0*10*2 -> out0.
    // w components are not transformed: out3.w = 0.5, out1.w = 1 + 0.25, out0.w = 2.
    float want[5][4] = { { 20, 40, 60, 2 }, { 8 + 10, 10 + 12.5f, 12 + 15, 1.25f }, { 0, 0, 0, 0 }, { 10, 11, 12, 0.5f }, { 0, 0, 0, 0 } };
    CHECK(memcmp(out, want, sizeof(want)) == 0);
}

int main()
{
    Run(false);
    Run(true);
    if (g_fail) { printf("TRANSFORM_VERTICES_FAIL %d\n", g_fail); return 1; }
    printf("TRANSFORM_VERTICES_OK\n");
    return 0;
}
