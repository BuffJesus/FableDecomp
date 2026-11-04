// Behaviour fixture for CParticleSimulation::ApplyForces (provider CParticleSimulation_ApplyForces_00adf760.cpp), reached
// through its mangled name (private). The simulation is raw storage at retail offsets (Timestep +0x0C, TimestepChanged
// +0x10, TimestepMultiplier +0x14, Size +0x18, Positions +0x1C, LastPositions +0x20, GravityStrength +0x34,
// WindStrength +0x38, AccelerationEnable +0x44, GlobalDamping +0x48). The fixture supplies a context with known gravity
// and wind (the CRT supplies pow). Checked: without acceleration, current positions are copied over the previous ones,
// the context is not asked, the arrays keep their roles; with acceleration, gravity and wind are each asked once, the
// two arrays swap roles, and every particle (all four components) becomes cur + (cur - prev) * damping + step with
// damping = (1 - GlobalDamping)^Timestep * TimestepMultiplier and step = (gravity * Gs + wind * Ws, 0) * Timestep^2
// (compared to 1e-5); in both cases TimestepChanged is cleared and TimestepMultiplier set to 1.
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "fable_particle_simulation.h"
#include "fable_particle_context.h"
#include "fable_4d_vector.h"
#include "fable_bounding_volumes.h"

static int g_fail, g_gravity, g_wind;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}

class CFixtureContext : public CParticleContext
{
public:
    C3DVector GetGravityVector() const { ++g_gravity; return C3DVector(0.0f, -9.0f, 1.0f); }
    C3DVector GetWindVector() const { ++g_wind; return C3DVector(2.0f, 0.5f, -1.0f); }
    void GetDeltaTransform(CMatrix3x4&) const { CHECK(!"not asked"); }
    long GetMaximumIterations() const { CHECK(!"not asked"); return 0; }
    float GetMaximumDistanceError() const { CHECK(!"not asked"); return 0; }
};

extern "C" void __fastcall FixForces(void* self, int, const CParticleContext* context);
#pragma comment(linker, "/alternatename:@FixForces@12=?ApplyForces@CParticleSimulation@@AAEXABVCParticleContext@@@Z")
static void PutF(unsigned char* raw, unsigned o, float v) { memcpy(raw + o, &v, 4); }
static void PutP(unsigned char* raw, unsigned o, const void* v) { memcpy(raw + o, &v, 4); }
static unsigned long Word(const unsigned char* raw, unsigned o) { unsigned long v; memcpy(&v, raw + o, 4); return v; }
static float Float(const unsigned char* raw, unsigned o) { float v; memcpy(&v, raw + o, 4); return v; }
static bool Near(float a, float b) { float d = a - b; if (d < 0) d = -d; float m = b < 0 ? -b : b; return d <= 1e-5f * (m > 1 ? m : 1); }

int main()
{
    static unsigned char sim[0x80];
    static float a[3][4], b[3][4], aBefore[3][4], bBefore[3][4];
    CFixtureContext context;
    for (int i = 0; i < 3; ++i) for (int k = 0; k < 4; ++k) { a[i][k] = 1.0f + i + 0.25f * k; b[i][k] = 0.5f * i - 0.125f * k; }
    memcpy(aBefore, a, sizeof(a)); memcpy(bBefore, b, sizeof(b));
    memset(sim, 0, sizeof(sim));
    unsigned long size = 3; memcpy(sim + 0x18, &size, 4);
    PutP(sim, 0x1C, a); PutP(sim, 0x20, b);
    PutF(sim, 0x0C, 0.0666667f); sim[0x10] = 1; PutF(sim, 0x14, 0.5f);
    PutF(sim, 0x34, 2.0f); PutF(sim, 0x38, 3.0f); PutF(sim, 0x48, 0.25f);

    // No acceleration: copy current over previous.
    FixForces(sim, 0, &context);
    CHECK(g_gravity == 0 && g_wind == 0);
    CHECK(Word(sim, 0x1C) == (unsigned long)a && Word(sim, 0x20) == (unsigned long)b);
    CHECK(memcmp(a, aBefore, sizeof(a)) == 0 && memcmp(b, aBefore, sizeof(b)) == 0);
    CHECK(sim[0x10] == 0 && Float(sim, 0x14) == 1.0f);

    // Acceleration: Verlet step.
    memcpy(b, bBefore, sizeof(b));
    sim[0x44] = 1; sim[0x10] = 1; PutF(sim, 0x14, 0.5f);
    FixForces(sim, 0, &context);
    CHECK(g_gravity == 1 && g_wind == 1);
    CHECK(Word(sim, 0x1C) == (unsigned long)b && Word(sim, 0x20) == (unsigned long)a);   // roles swapped
    float t = 0.0666667f, damping = (float)pow(1.0f - 0.25f, t) * 0.5f, t2 = t * t;
    float step[4] = { (0.0f * 2 + 2.0f * 3) * t2, (-9.0f * 2 + 0.5f * 3) * t2, (1.0f * 2 - 1.0f * 3) * t2, 0.0f };
    for (int i = 0; i < 3; ++i)
        for (int k = 0; k < 4; ++k)
        {
            float cur = aBefore[i][k], prev = bBefore[i][k];
            CHECK(Near(b[i][k], cur + (cur - prev) * damping + step[k]));
            CHECK(a[i][k] == cur);   // the current positions became the previous ones, unchanged
        }
    CHECK(sim[0x10] == 0 && Float(sim, 0x14) == 1.0f);

    if (g_fail) { printf("APPLY_FORCES_FAIL %d\n", g_fail); return 1; }
    printf("APPLY_FORCES_OK\n");
    return 0;
}
