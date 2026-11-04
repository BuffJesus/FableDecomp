// Behaviour fixture for CParticleSimulation::Clear (provider CParticleSimulation_Clear_00adf1e0.cpp). The simulation is raw
// storage at retail offsets; the fixture supplies a recording CMovableResourceMemoryPool::FreeResource. Clear must restore
// the defaults (timestep 1/15, multiplier 1, damping 0.25, time 0, no arrays, no adjustment, timestep unchanged), free an
// allocated handle exactly once through the simulation's own pool and mark it unallocated, and leave every other member
// (render data, size, forces, dragging, bounding box) alone.
#include <stdio.h>
#include <string.h>
#include "fable_particle_simulation.h"

static int g_fail, g_frees;
static const void* g_freePool;
static long g_freeHandle;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void CMovableResourceMemoryPool::FreeResource(long handle) { ++g_frees; g_freePool = this; g_freeHandle = handle; }

int main()
{
    static unsigned char raw[0x80], before[0x80];
    for (int i = 0; i < 0x80; ++i) raw[i] = (unsigned char)(0x90 + i);
    long handle = 12; memcpy(raw + 0x74, &handle, 4);
    static unsigned char pool[16]; void* p = pool; memcpy(raw + 0x78, &p, 4);
    memcpy(before, raw, sizeof(raw));
    CParticleSimulation* s = reinterpret_cast<CParticleSimulation*>(raw);

    s->Clear();
    CHECK(s->Timestep == 1.0f / 15.0f && s->TimestepMultiplier == 1.0f && s->GlobalDamping == 0.25f && s->CurrentTime == 0.0);
    CHECK(!s->TimestepChanged && s->MaximumAdjustment == 0.0f);
    CHECK(!s->Positions && !s->LastPositions && !s->SavedPositions && !s->Accelerations && !s->SimulationAlphas);
    CHECK(g_frees == 1 && g_freePool == pool && g_freeHandle == 12 && s->MemoryHandle == -1);
    CHECK(memcmp(raw + 0x04, before + 0x04, 8) == 0 && memcmp(raw + 0x18, before + 0x18, 4) == 0);   // render data, size
    CHECK(memcmp(raw + 0x34, before + 0x34, 0x14) == 0);                                             // forces, dragging
    CHECK(memcmp(raw + 0x58, before + 0x58, 0x1C) == 0 && memcmp(raw + 0x78, before + 0x78, 4) == 0); // bbox, pool

    s->Clear();   // nothing allocated any more: no second free
    CHECK(g_frees == 1 && s->MemoryHandle == -1);

    if (g_fail) { printf("PARTICLE_SIMULATION_CLEAR_FAIL %d\n", g_fail); return 1; }
    printf("PARTICLE_SIMULATION_CLEAR_OK\n");
    return 0;
}
