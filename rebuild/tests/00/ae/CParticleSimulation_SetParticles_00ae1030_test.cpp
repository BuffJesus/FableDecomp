// Behaviour fixture for CParticleSimulation::SetParticles and CParticleProgram::SetData (provider
// CParticleSimulation_SetParticles_00ae1030.cpp). Objects are raw storage at retail offsets; the input arrays are raw
// STLport vectors. The fixture supplies both AllocateArrays (recording; they point the arrays at fixture buffers; the
// program's can be told to fail). Checked: SetParticles sets Size from the position count before allocating, then for
// every particle writes (x, y, z, 0) to Positions, LastPositions and SavedPositions, a zero acceleration, and the
// particle's alpha; it still fills the arrays it was given when the allocation reports failure. SetData sets NonSimCount
// before allocating that many, copies each vertex as (x, y, z, 0) and returns true; on failure it returns false without
// writing.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"
#include "fable_4d_vector.h"

static int g_fail, g_simAllocs, g_progAllocs; static bool g_ok;
static unsigned long g_sizeAtAlloc, g_progCount; static const void* g_simPool; static const void* g_progPool;
static float g_arr[4][4][4], g_alpha[4], g_nonSim[4][4];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static void Put(void* raw, unsigned o, const void* v) { memcpy((unsigned char*)raw + o, &v, 4); }
bool CParticleSimulation::AllocateArrays(CMovableResourceMemoryPool& pool)
{
    ++g_simAllocs; g_simPool = &pool; g_sizeAtAlloc = Size;
    for (int i = 0; i < 4; ++i) Put(this, 0x1C + 4 * i, g_arr[i]);
    Put(this, 0x2C, g_alpha);
    return g_ok;
}
bool CParticleProgram::AllocateArrays(unsigned long count, CMovableResourceMemoryPool& pool)
{
    ++g_progAllocs; g_progCount = count; g_progPool = &pool;
    if (!g_ok) return false;
    Put(this, 0xC0, g_nonSim); return true;
}
static void Vec(unsigned long* v, void* b, unsigned n, unsigned elem)
{
    unsigned char* s = (unsigned char*)b; unsigned char* e = s + n * elem; memcpy(v, &s, 4); memcpy(v + 1, &e, 4); memcpy(v + 2, &e, 4);
}

int main()
{
    static unsigned char sim[0x80], program[0x140], pool[16];
    static float pos[3][3] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    static float alphas[3] = { 0.5f, 1.0f, 0.25f };
    unsigned long pv[3], av[3]; Vec(pv, pos, 3, 12); Vec(av, alphas, 3, 4);
    CParticleSimulation& s = *reinterpret_cast<CParticleSimulation*>(sim);
    CMovableResourceMemoryPool& mp = *reinterpret_cast<CMovableResourceMemoryPool*>(pool);
    const CArray<C3DVector>& P = *reinterpret_cast<const CArray<C3DVector>*>(pv);
    const CArray<float>& A = *reinterpret_cast<const CArray<float>*>(av);

    for (int pass = 0; pass < 2; ++pass)
    {
        g_ok = pass == 0; memset(g_arr, 0x55, sizeof(g_arr)); memset(g_alpha, 0x55, sizeof(g_alpha));
        s.SetParticles(P, A, mp);
        CHECK(g_simAllocs == pass + 1 && g_sizeAtAlloc == 3 && g_simPool == pool);
        for (int i = 0; i < 3; ++i)
        {
            float want[4] = { pos[i][0], pos[i][1], pos[i][2], 0.0f }, zero[4] = { 0, 0, 0, 0 };
            for (int a = 0; a < 3; ++a) CHECK(memcmp(g_arr[a][i], want, 16) == 0);
            CHECK(memcmp(g_arr[3][i], zero, 16) == 0 && g_alpha[i] == alphas[i]);
        }
        unsigned char untouched[16]; memset(untouched, 0x55, 16);
        CHECK(memcmp(g_arr[0][3], untouched, 16) == 0);
    }

    g_ok = true; memset(g_nonSim, 0x55, sizeof(g_nonSim));
    CParticleProgram* p = reinterpret_cast<CParticleProgram*>(program);
    CHECK(p->SetData(P, mp));
    CHECK(g_progAllocs == 1 && g_progCount == 3 && g_progPool == pool);
    unsigned long n; memcpy(&n, program + 0xBC, 4); CHECK(n == 3);
    for (int i = 0; i < 3; ++i) { float want[4] = { pos[i][0], pos[i][1], pos[i][2], 0.0f }; CHECK(memcmp(g_nonSim[i], want, 16) == 0); }
    g_ok = false; memset(g_nonSim, 0x55, sizeof(g_nonSim)); memset(program + 0xC0, 0, 4);
    CHECK(!p->SetData(P, mp));
    CHECK(g_progAllocs == 2);

    if (g_fail) { printf("SET_PARTICLES_FAIL %d\n", g_fail); return 1; }
    printf("SET_PARTICLES_OK\n");
    return 0;
}
