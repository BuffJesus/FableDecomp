// Behaviour fixture for both CParticleProgram::PrepareForRendering overloads (provider
// CParticleProgram_PrepareForRendering_00ae1260.cpp). Program and simulation are raw storage at retail offsets (program
// InitialSimulation.Positions +0x24, NonSimCount +0xBC, NonSimPositions +0xC0, SimGroups +0xFC, NonSimGroups +0x108;
// simulation Size +0x18, Positions +0x1C, SavedPositions +0x24). The fixture supplies TransformVertices, both
// GFInterpolateVectors overloads, GetVectorBuffer and DELTA_NOTIONAL_ZERO (all recording). Checked: the static overload
// copies exactly NonSimCount non-simulated vectors to the front of the output; both overloads then interpolate the
// particles after them from SavedPositions to Positions by alpha (strides 16, Size particles); a blend at or below
// DELTA_NOTIONAL_ZERO stops there; above it the particles are moved towards the initial positions by blend - directly
// (static) or through the vector scratch buffer filled by transforming the initial positions with the SimGroups
// (skinned, whose non-simulated vertices are transformed with the NonSimGroups first).
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"
#include "fable_vector_math.h"

extern const float DELTA_NOTIONAL_ZERO = 0.0001f;
static int g_fail, g_step, g_transforms, g_lerps2, g_lerps1, g_buffers;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

struct TCall { const void *self, *out, *in, *matrices, *groups; unsigned long count; int step; } g_t[4];
struct L2 { const void *out, *from, *to; unsigned long strides[3], count; float t; int step; } g_l2;
struct L1 { const void *out, *to; unsigned long strides[2], count; float t; int step; } g_l1;
static unsigned char g_buffer[256]; static unsigned long g_bufferCount; static int g_bufferStep;

void CParticleProgram::TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* m,
                                         const CArray<C3DGroup2>& groups) const
{
    TCall c = { this, out, in, m, &groups, count, ++g_step }; g_t[g_transforms++ & 3] = c;
}
void NVectorMath::GFInterpolateVectors(C4DVector* out, unsigned long os, const C4DVector* from, unsigned long fs,
                                       const C4DVector* to, unsigned long ts, unsigned long count, float t)
{
    ++g_lerps2; L2 c = { out, from, to, { os, fs, ts }, count, t, ++g_step }; g_l2 = c;
}
void NVectorMath::GFInterpolateVectors(C4DVector* out, unsigned long os, const C4DVector* to, unsigned long ts,
                                       unsigned long count, float t)
{
    ++g_lerps1; L1 c = { out, to, { os, ts }, count, t, ++g_step }; g_l1 = c;
}
C4DVector* FABLE_FASTCALL CParticleProgram::GetVectorBuffer(unsigned long count)
{
    ++g_buffers; g_bufferCount = count; g_bufferStep = ++g_step; return reinterpret_cast<C4DVector*>(g_buffer);
}

static void Put(unsigned char* raw, unsigned o, const void* v) { memcpy(raw + o, &v, 4); }
static void PutW(unsigned char* raw, unsigned o, unsigned long v) { memcpy(raw + o, &v, 4); }
static void Reset() { g_step = g_transforms = g_lerps2 = g_lerps1 = g_buffers = 0; memset(&g_l1, 0, sizeof(g_l1)); memset(&g_l2, 0, sizeof(g_l2)); }

int main()
{
    static unsigned char program[0x140], sim[0x80], nonSim[2 * 16], initial[16], saved[16], current[16], out[8 * 16];
    for (int i = 0; i < (int)sizeof(nonSim); ++i) nonSim[i] = (unsigned char)(i + 1);
    PutW(program, 0xBC, 2); Put(program, 0xC0, nonSim); Put(program, 0x08 + 0x1C, initial);
    PutW(sim, 0x18, 3); Put(sim, 0x1C, current); Put(sim, 0x24, saved);
    CParticleProgram* p = reinterpret_cast<CParticleProgram*>(program);
    CParticleSimulation& s = *reinterpret_cast<CParticleSimulation*>(sim);

    // Static, no blend.
    memset(out, 0xEE, sizeof(out)); Reset();
    p->PrepareForRendering(reinterpret_cast<C4DVector*>(out), s, 0.25f, 0.0001f);
    CHECK(memcmp(out, nonSim, sizeof(nonSim)) == 0 && out[sizeof(nonSim)] == 0xEE);
    CHECK(g_lerps2 == 1 && g_lerps1 == 0 && g_transforms == 0 && g_buffers == 0);
    CHECK(g_l2.out == out + 32 && g_l2.from == saved && g_l2.to == current && g_l2.count == 3 && g_l2.t == 0.25f);
    CHECK(g_l2.strides[0] == 16 && g_l2.strides[1] == 16 && g_l2.strides[2] == 16);
    // Static, blended.
    Reset();
    p->PrepareForRendering(reinterpret_cast<C4DVector*>(out), s, 0.5f, 0.75f);
    CHECK(g_lerps2 == 1 && g_lerps1 == 1 && g_l2.step < g_l1.step);
    CHECK(g_l1.out == out + 32 && g_l1.to == initial && g_l1.count == 3 && g_l1.t == 0.75f && g_l1.strides[0] == 16 && g_l1.strides[1] == 16);

    // Skinned.
    static unsigned char blendA[4], blendB[4], anim[4], verts[12], matrices[64], mesh[4];
    const C3DAnimationBlendState& ba = *reinterpret_cast<const C3DAnimationBlendState*>(blendA);
    const C3DAnimationBlendState& bb = *reinterpret_cast<const C3DAnimationBlendState*>(blendB);
    C3DAnimationManager& am = *reinterpret_cast<C3DAnimationManager*>(anim);
    const C3DMesh2& me = *reinterpret_cast<const C3DMesh2*>(mesh);
    const CMatrix4x4* mx = reinterpret_cast<const CMatrix4x4*>(matrices);
    Reset();
    p->PrepareForRendering(reinterpret_cast<C4DVector*>(out), s, 0.25f, 0.0f, ba, bb, 0.5f, am,
                           reinterpret_cast<const C3DVector*>(verts), mx, me);
    CHECK(g_transforms == 1 && g_lerps2 == 1 && g_lerps1 == 0 && g_buffers == 0);
    CHECK(g_t[0].self == program && g_t[0].out == out && g_t[0].in == nonSim && g_t[0].count == 2 && g_t[0].matrices == matrices);
    CHECK(g_t[0].groups == program + 0x108 && g_t[0].step < g_l2.step);
    CHECK(g_l2.out == out + 32 && g_l2.from == saved && g_l2.to == current && g_l2.count == 3 && g_l2.t == 0.25f);
    Reset();
    p->PrepareForRendering(reinterpret_cast<C4DVector*>(out), s, 0.25f, 0.5f, ba, bb, 0.5f, am,
                           reinterpret_cast<const C3DVector*>(verts), mx, me);
    CHECK(g_transforms == 2 && g_lerps2 == 1 && g_lerps1 == 1 && g_buffers == 1 && g_bufferCount == 3);
    CHECK(g_t[1].out == g_buffer && g_t[1].in == initial && g_t[1].count == 3 && g_t[1].matrices == matrices && g_t[1].groups == program + 0xFC);
    CHECK(g_l2.step < g_bufferStep && g_bufferStep < g_t[1].step && g_t[1].step < g_l1.step);
    CHECK(g_l1.out == out + 32 && g_l1.to == g_buffer && g_l1.count == 3 && g_l1.t == 0.5f);

    if (g_fail) { printf("PREPARE_FOR_RENDERING_FAIL %d\n", g_fail); return 1; }
    printf("PREPARE_FOR_RENDERING_OK\n");
    return 0;
}
