// Behaviour fixture for both CParticleProgram::SimulationStep overloads (provider CParticleProgram_SimulationStep_00ae2cb0.cpp).
// Program and simulation are raw storage at retail offsets (simulation Size +0x18, Positions +0x1C, SimulationAlphas
// +0x2C; the program's initial simulation at +0x08). The fixture supplies ApplyDragging, ApplyForces,
// NVectorMath::GFInterpolateVectors, ClingToMesh and ApplyConstraints (all recording call order and arguments) and a
// context whose iteration / distance-error limits are known. Checked: an empty simulation does nothing; otherwise
// dragging, forces, then (free-standing) one interpolation of the simulation's positions towards the initial
// simulation's positions by the simulation alphas over the initial simulation's particle count with strides 16/16/16/4,
// or (clinging) one ClingToMesh with every mesh argument passed through, and finally the constraints with the context's
// limits - each limit asked for exactly once.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"
#include "fable_particle_context.h"
#include "fable_vector_math.h"
#include "fable_bounding_volumes.h"

static int g_fail, g_step, g_drag, g_forces, g_interp, g_cling, g_constraints, g_iterAsks, g_errorAsks;
static int g_dragStep, g_forcesStep, g_interpStep, g_clingStep, g_constraintsStep;
static const void *g_dragThis, *g_dragContext, *g_forcesThis, *g_forcesContext;
static const void *g_out, *g_from, *g_to, *g_t; static unsigned long g_strides[4], g_count;
static const void *g_clingThis, *g_clingSim, *g_blend, *g_anim, *g_verts, *g_mesh;
static const void *g_conThis, *g_conSim; static long g_conIterations; static float g_conError;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}

void CParticleSimulation::ApplyDragging(const CParticleContext& c) { ++g_drag; g_dragStep = ++g_step; g_dragThis = this; g_dragContext = &c; }
void CParticleSimulation::ApplyForces(const CParticleContext& c) { ++g_forces; g_forcesStep = ++g_step; g_forcesThis = this; g_forcesContext = &c; }
void NVectorMath::GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride,
                                       const C4DVector* to, unsigned long toStride, const float* t, unsigned long tStride, unsigned long count)
{
    ++g_interp; g_interpStep = ++g_step; g_out = out; g_from = from; g_to = to; g_t = t;
    g_strides[0] = outStride; g_strides[1] = fromStride; g_strides[2] = toStride; g_strides[3] = tStride; g_count = count;
}
void CParticleProgram::ClingToMesh(CParticleSimulation& s, const C3DAnimationBlendState& b, C3DAnimationManager& a,
                                   const C3DVector* v, const C3DMesh2& m) const
{
    ++g_cling; g_clingStep = ++g_step; g_clingThis = this; g_clingSim = &s; g_blend = &b; g_anim = &a; g_verts = v; g_mesh = &m;
}
void CParticleProgram::ApplyConstraints(CParticleSimulation& s, long iterations, float error) const
{
    ++g_constraints; g_constraintsStep = ++g_step; g_conThis = this; g_conSim = &s; g_conIterations = iterations; g_conError = error;
}

class CFixtureContext : public CParticleContext
{
public:
    C3DVector GetGravityVector() const { CHECK(!"gravity not asked"); return C3DVector(); }
    C3DVector GetWindVector() const { CHECK(!"wind not asked"); return C3DVector(); }
    void GetDeltaTransform(CMatrix3x4&) const { CHECK(!"delta not asked"); }
    long GetMaximumIterations() const { ++g_iterAsks; return 7; }
    float GetMaximumDistanceError() const { ++g_errorAsks; return 0.125f; }
};

static void Put(unsigned char* raw, unsigned offset, const void* value) { memcpy(raw + offset, &value, 4); }
static void PutWord(unsigned char* raw, unsigned offset, unsigned long value) { memcpy(raw + offset, &value, 4); }

int main()
{
    static unsigned char program[0x140], programBefore[0x140], sim[0x80], simBefore[0x80];
    static unsigned char positions[64], initialPositions[64], alphas[16], blend[4], anim[4], verts[12], mesh[4];
    memset(program, 0x44, sizeof(program));
    PutWord(program, 0x08 + 0x18, 5); Put(program, 0x08 + 0x1C, initialPositions);
    memset(sim, 0x55, sizeof(sim));
    Put(sim, 0x1C, positions); Put(sim, 0x2C, alphas);
    const CParticleProgram* p = reinterpret_cast<const CParticleProgram*>(program);
    CParticleSimulation& s = *reinterpret_cast<CParticleSimulation*>(sim);
    CFixtureContext context;
    const C3DAnimationBlendState& b = *reinterpret_cast<const C3DAnimationBlendState*>(blend);
    C3DAnimationManager& a = *reinterpret_cast<C3DAnimationManager*>(anim);
    const C3DMesh2& m = *reinterpret_cast<const C3DMesh2*>(mesh);
    const C3DVector* v = reinterpret_cast<const C3DVector*>(verts);

    // Empty simulation: nothing.
    PutWord(sim, 0x18, 0);
    p->SimulationStep(s, context);
    p->SimulationStep(s, context, b, a, v, m);
    CHECK(g_step == 0 && g_iterAsks == 0 && g_errorAsks == 0);

    // Free-standing step.
    PutWord(sim, 0x18, 3);
    memcpy(programBefore, program, sizeof(program)); memcpy(simBefore, sim, sizeof(sim));
    p->SimulationStep(s, context);
    CHECK(g_drag == 1 && g_forces == 1 && g_interp == 1 && g_cling == 0 && g_constraints == 1);
    CHECK(g_dragStep == 1 && g_forcesStep == 2 && g_interpStep == 3 && g_constraintsStep == 4);
    CHECK(g_dragThis == sim && g_dragContext == &context && g_forcesThis == sim && g_forcesContext == &context);
    CHECK(g_out == positions && g_from == initialPositions && g_to == positions && g_t == alphas && g_count == 5);
    CHECK(g_strides[0] == 16 && g_strides[1] == 16 && g_strides[2] == 16 && g_strides[3] == 4);
    CHECK(g_conThis == program && g_conSim == sim && g_conIterations == 7 && g_conError == 0.125f);
    CHECK(g_iterAsks == 1 && g_errorAsks == 1);
    CHECK(memcmp(program, programBefore, sizeof(program)) == 0 && memcmp(sim, simBefore, sizeof(sim)) == 0);

    // Clinging step.
    g_step = 0;
    p->SimulationStep(s, context, b, a, v, m);
    CHECK(g_drag == 2 && g_forces == 2 && g_interp == 1 && g_cling == 1 && g_constraints == 2);
    CHECK(g_dragStep == 1 && g_forcesStep == 2 && g_clingStep == 3 && g_constraintsStep == 4);
    CHECK(g_clingThis == program && g_clingSim == sim && g_blend == blend && g_anim == anim && g_verts == verts && g_mesh == mesh);
    CHECK(g_conThis == program && g_conSim == sim && g_conIterations == 7 && g_conError == 0.125f);
    CHECK(g_iterAsks == 2 && g_errorAsks == 2);

    if (g_fail) { printf("SIMULATION_STEP_FAIL %d\n", g_fail); return 1; }
    printf("SIMULATION_STEP_OK\n");
    return 0;
}
