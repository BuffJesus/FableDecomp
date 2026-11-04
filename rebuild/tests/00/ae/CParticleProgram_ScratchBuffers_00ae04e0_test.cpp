// Behaviour fixture for CParticleProgram's static scratch buffers GetMatrixBuffer / GetVectorBuffer /
// GetGroupVectorBuffer (provider CParticleProgram_ScratchBuffers_00ae04e0.cpp). The fixture supplies CSimpleMemoryPool
// (recording construction arguments, FreeAll and Allocate per pool; Allocate hands out distinct fake blocks).
// Checked: the provider constructs three distinct pools as (capacity 0, growth 0x1000); each getter uses one pool of its
// own; a first request frees its pool and allocates count * element size (64 for matrices, 16 for vectors); a request
// that is not larger than the current size returns the same block without touching the pool; a larger one frees and
// reallocates; a count whose byte size overflows 32 bits frees the pool, allocates nothing and returns 0, and the size
// still grows so a smaller request afterwards returns that 0 without a new allocation.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"
#include "fable_matrix4x4.h"
#include "fable_simple_memory_pool.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

struct Record { const void* pool; unsigned long capacity, growth; int frees, allocs; unsigned long lastSize; };
static Record g_pools[4];
static int g_poolCount;
static unsigned char g_heap[16][16];
static int g_next;
static Record* Find(const void* pool)
{
    for (int i = 0; i < g_poolCount; ++i) if (g_pools[i].pool == pool) return &g_pools[i];
    return 0;
}

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CSimpleMemoryPool::CSimpleMemoryPool(unsigned long capacity, unsigned long growth)
{
    if (g_poolCount < 4) { Record r = { this, capacity, growth, 0, 0, 0 }; g_pools[g_poolCount++] = r; }
}
CSimpleMemoryPool::~CSimpleMemoryPool() {}
void CSimpleMemoryPool::FreeAll() { Record* r = Find(this); if (r) ++r->frees; }
void* CSimpleMemoryPool::Allocate(unsigned long size)
{
    Record* r = Find(this); if (r) { ++r->allocs; r->lastSize = size; }
    return g_heap[g_next++ & 15];
}

// The getters are private static __fastcall members: reach them by their mangled names.
extern "C" void* __fastcall FixMatrix(unsigned long count);
extern "C" void* __fastcall FixVector(unsigned long count);
extern "C" void* __fastcall FixGroupVector(unsigned long count);
#pragma comment(linker, "/alternatename:@FixMatrix@4=?GetMatrixBuffer@CParticleProgram@@CIPAVCMatrix4x4@@K@Z")
#pragma comment(linker, "/alternatename:@FixVector@4=?GetVectorBuffer@CParticleProgram@@CIPAVC4DVector@@K@Z")
#pragma comment(linker, "/alternatename:@FixGroupVector@4=?GetGroupVectorBuffer@CParticleProgram@@CIPAVC4DVector@@K@Z")

typedef void* (__fastcall* Getter)(unsigned long);

static void Exercise(Getter get, unsigned long element, unsigned long overflowCount)
{
    int before = g_poolCount;
    void* a = get(4);
    // Find the pool this getter used (the one with exactly one allocation of 4 elements).
    Record* r = 0;
    for (int i = 0; i < g_poolCount; ++i) if (g_pools[i].allocs == 1 && g_pools[i].lastSize == 4 * element && g_pools[i].frees == 1) r = &g_pools[i];
    CHECK(r != 0 && g_poolCount == before);
    if (!r) return;
    CHECK(a != 0);
    CHECK(get(4) == a && get(1) == a && r->frees == 1 && r->allocs == 1);
    CHECK(get(0) == a && r->allocs == 1);
    void* b = get(9);
    CHECK(b != 0 && b != a && r->frees == 2 && r->allocs == 2 && r->lastSize == 9 * element);
    // Overflow: freed, nothing allocated, 0 returned; the size still grew.
    CHECK(get(overflowCount) == 0 && r->frees == 3 && r->allocs == 2);
    CHECK(get(10) == 0 && r->frees == 3 && r->allocs == 2);
    // A still larger request (0xFFFFFFFF elements) frees again and overflows again.
    CHECK(get(0xFFFFFFFFu) == 0 && r->frees == 4 && r->allocs == 2);
    r->pool = (const void*)~(unsigned long)r->pool;   // retire it so the next Exercise cannot pick it again
}

int main()
{
    // Static construction happened before main: three distinct pools, each (0, 0x1000).
    CHECK(g_poolCount == 3);
    for (int i = 0; i < g_poolCount; ++i)
    {
        CHECK(g_pools[i].capacity == 0 && g_pools[i].growth == 0x1000 && g_pools[i].frees == 0 && g_pools[i].allocs == 0);
        for (int j = 0; j < i; ++j) CHECK(g_pools[i].pool != g_pools[j].pool);
    }
    // 0x04000000 * 64 and 0x10000000 * 16 are exactly 2^32.
    Exercise(FixMatrix, 64, 0x04000000u);
    Exercise(FixVector, 16, 0x10000000u);
    Exercise(FixGroupVector, 16, 0x10000000u);
    // Each getter retired a different pool.
    int retired = 0;
    for (int i = 0; i < g_poolCount; ++i) if ((unsigned long)g_pools[i].pool >= 0x80000000u) ++retired;
    CHECK(retired == 3);

    if (g_fail) { printf("SCRATCH_BUFFERS_FAIL %d\n", g_fail); return 1; }
    printf("SCRATCH_BUFFERS_OK\n");
    return 0;
}
