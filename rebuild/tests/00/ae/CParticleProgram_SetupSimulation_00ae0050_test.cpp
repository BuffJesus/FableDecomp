#include <cstdio>

struct CMovableResourceMemoryPool;

static void* g_captured_field = 0;
static CMovableResourceMemoryPool* g_captured_pool = 0;
static bool g_ret = false;

struct CParticleSimulation {
    char Init(void* progField, CMovableResourceMemoryPool* pool);
};

char CParticleSimulation::Init(void* progField, CMovableResourceMemoryPool* pool)
{
    g_captured_field = progField;
    g_captured_pool = pool;
    return g_ret;
}

struct CParticleProgram {
    char pad[8];
    bool SetupSimulation(CParticleSimulation* sim, CMovableResourceMemoryPool* pool);
};

int main()
{
    CParticleProgram prog;
    CParticleSimulation sim;
    CMovableResourceMemoryPool* pool = (CMovableResourceMemoryPool*)0xABCD;

    g_ret = true;
    bool r1 = prog.SetupSimulation(&sim, pool);

    g_ret = 0;
    bool r2 = prog.SetupSimulation(&sim, pool);

    bool ok = (r1 == true) && (r2 == false)
           && (g_captured_field == (void*)((char*)&prog + 8))
           && (g_captured_pool == pool);

    if (ok) printf("SETUPSIM_OK\n");
    else printf("SETUPSIM_FAIL f=%p p=%p r1=%d r2=%d\n", g_captured_field, g_captured_pool, r1, r2);
    return ok ? 0 : 1;
}