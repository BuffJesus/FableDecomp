// Behaviour fixture for CParticleSimulation::ApplyDragging (provider CParticleSimulation_ApplyDragging_00adf300.cpp),
// reached through its mangled name (private). The simulation is raw storage at retail offsets (Size +0x18, Positions
// +0x1C, LastPositions +0x20, DraggingEnable +0x3C, DraggingStrength +0x40). The fixture supplies a context whose delta
// transform is a known 3x4 matrix, CMatrix4x4::Identity, AlignedSetInterpolate and AlignedTransformTransposedAssumeW
// (recording). Checked: dragging disabled does nothing (the context is not asked); enabled, the delta is converted to a
// transposed 4x4 (3x4 rows become columns, last row 0 0 0 1) on a 16-byte aligned local, interpolated in place towards
// Identity by 1 - DraggingStrength, then applied in place to Positions and to LastPositions (Size each, w = 1) in that
// order, both through that same matrix.
#include <stdio.h>
#include <string.h>
#include "fable_particle_simulation.h"
#include "fable_particle_context.h"
#include "fable_matrix4x4.h"
#include "fable_bounding_volumes.h"

static int g_fail, g_deltas, g_interps, g_transforms;
static const void *g_interpThis, *g_interpA, *g_interpB; static float g_interpT; static float g_seen[16];
static const void *g_tThis[2], *g_tIn[2], *g_tOut[2]; static unsigned long g_tCount[2]; static float g_tW[2];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CMatrix4x4 CMatrix4x4::Identity;
void CMatrix4x4::AlignedSetInterpolate(const CMatrix4x4& a, const CMatrix4x4& b, float t)
{
    ++g_interps; g_interpThis = this; g_interpA = &a; g_interpB = &b; g_interpT = t; memcpy(g_seen, this, 64);
}
void CMatrix4x4::AlignedTransformTransposedAssumeW(const C4DVector* in, C4DVector* out, unsigned long count, float w) const
{
    int i = g_transforms++ & 1; g_tThis[i] = this; g_tIn[i] = in; g_tOut[i] = out; g_tCount[i] = count; g_tW[i] = w;
}

class CFixtureContext : public CParticleContext
{
public:
    C3DVector GetGravityVector() const { CHECK(!"not asked"); return C3DVector(); }
    C3DVector GetWindVector() const { CHECK(!"not asked"); return C3DVector(); }
    void GetDeltaTransform(CMatrix3x4& t) const { ++g_deltas; float* f = &t.E11; for (int i = 0; i < 12; ++i) f[i] = (float)(i + 1); }
    long GetMaximumIterations() const { CHECK(!"not asked"); return 0; }
    float GetMaximumDistanceError() const { CHECK(!"not asked"); return 0; }
};

extern "C" void __fastcall FixDrag(void* self, int, const CParticleContext* context);
#pragma comment(linker, "/alternatename:@FixDrag@12=?ApplyDragging@CParticleSimulation@@AAEXABVCParticleContext@@@Z")
static void Put(unsigned char* raw, unsigned offset, const void* value, unsigned n) { memcpy(raw + offset, value, n); }

int main()
{
    static unsigned char sim[0x80], before[0x80], positions[64], last[64];
    CFixtureContext context;
    memset(sim, 0, sizeof(sim));
    unsigned long size = 4; void* p = positions; void* l = last; float strength = 0.25f;
    Put(sim, 0x18, &size, 4); Put(sim, 0x1C, &p, 4); Put(sim, 0x20, &l, 4); Put(sim, 0x40, &strength, 4);
    memcpy(before, sim, sizeof(sim));

    FixDrag(sim, 0, &context);
    CHECK(g_deltas == 0 && g_interps == 0 && g_transforms == 0 && memcmp(sim, before, sizeof(sim)) == 0);

    sim[0x3C] = 1;
    FixDrag(sim, 0, &context);
    CHECK(g_deltas == 1 && g_interps == 1 && g_transforms == 2);
    CHECK(((unsigned long)g_interpThis & 15) == 0 && g_interpA == g_interpThis && g_interpB == &CMatrix4x4::Identity);
    CHECK(g_interpT == 0.75f);
    // 3x4 E11..E43 = 1..12: E11 1 E12 2 E13 3 / E21 4 E22 5 E23 6 / E31 7 E32 8 E33 9 / E41 10 E42 11 E43 12.
    static const float want[16] = { 1, 4, 7, 10,  2, 5, 8, 11,  3, 6, 9, 12,  0, 0, 0, 1 };
    CHECK(memcmp(g_seen, want, 64) == 0);
    CHECK(g_tThis[0] == g_interpThis && g_tIn[0] == positions && g_tOut[0] == positions && g_tCount[0] == 4 && g_tW[0] == 1.0f);
    CHECK(g_tThis[1] == g_interpThis && g_tIn[1] == last && g_tOut[1] == last && g_tCount[1] == 4 && g_tW[1] == 1.0f);

    if (g_fail) { printf("APPLY_DRAGGING_FAIL %d\n", g_fail); return 1; }
    printf("APPLY_DRAGGING_OK\n");
    return 0;
}
