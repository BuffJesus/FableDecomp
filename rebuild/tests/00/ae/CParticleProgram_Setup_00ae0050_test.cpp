// Behaviour fixture for CParticleProgram::SetupSimulation (provider CParticleProgram_Setup_00ae0050.cpp). The program is
// raw storage; the fixture supplies CParticleSimulation::CopyFrom (recording; its result can be chosen). Checked: the
// target simulation copies from the program's initial simulation (program + 0x08) with the given pool, and the copy's
// result is returned unchanged in both directions.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"

static int g_fail, g_calls;
static bool g_result;
static const void *g_this, *g_source, *g_pool;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

bool CParticleSimulation::CopyFrom(const CParticleSimulation& source, CMovableResourceMemoryPool& pool)
{
    ++g_calls; g_this = this; g_source = &source; g_pool = &pool;
    return g_result;
}

int main()
{
    static unsigned char program[0x140], before[0x140], sim[0x80], pool[0x40];
    memset(program, 0x3C, sizeof(program)); memcpy(before, program, sizeof(program));
    const CParticleProgram* p = reinterpret_cast<const CParticleProgram*>(program);
    CParticleSimulation& s = *reinterpret_cast<CParticleSimulation*>(sim);
    CMovableResourceMemoryPool& mp = *reinterpret_cast<CMovableResourceMemoryPool*>(pool);

    g_result = true;
    CHECK(p->SetupSimulation(s, mp) == true);
    CHECK(g_calls == 1 && g_this == sim && g_source == program + 0x08 && g_pool == pool);
    g_result = false;
    CHECK(p->SetupSimulation(s, mp) == false);
    CHECK(g_calls == 2 && g_this == sim && g_source == program + 0x08 && g_pool == pool);
    CHECK(memcmp(program, before, sizeof(program)) == 0);

    if (g_fail) { printf("SETUP_SIMULATION_FAIL %d\n", g_fail); return 1; }
    printf("SETUP_SIMULATION_OK\n");
    return 0;
}
