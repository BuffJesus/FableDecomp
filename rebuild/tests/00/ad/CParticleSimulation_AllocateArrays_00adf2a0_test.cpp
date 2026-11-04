// Behaviour fixture for CParticleSimulation::AllocateArrays (provider CParticleSimulation_AllocateArrays_00adf2a0.cpp).
// The simulation is raw storage at retail offsets; the fixture supplies the pool's AllocateResource and
// GetPointerFromHandle (recording; allocation can be told to fail). Checked: the pool is remembered whether or not the
// allocation succeeds; the request is for Size * 0x44 bytes against the simulation's MemoryHandle, owned by the
// simulation itself, with no flags; on success the five arrays are consecutive slices of the block (four vector arrays
// of Size, then the alphas) and true is returned; on failure the arrays are untouched and false is returned.
#include <stdio.h>
#include <string.h>
#include "fable_particle_simulation.h"

static int g_fail, g_allocs, g_lookups;
static bool g_ok = true;
static const void* g_allocPool;
static const void* g_lookupPool;
static long* g_handleSlot;
static const void* g_owner;
static unsigned long g_size, g_flags;
static long g_lookupHandle;
static unsigned char g_block[0x1000];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

bool CMovableResourceMemoryPool::AllocateResource(long* handle, CMovableResource* owner, unsigned long size, unsigned long flags)
{
    ++g_allocs; g_allocPool = this; g_handleSlot = handle; g_owner = owner; g_size = size; g_flags = flags;
    if (!g_ok) return false;
    *handle = 77;
    return true;
}
void* CHandleBasedMemoryPool::GetPointerFromHandle(long handle) { ++g_lookups; g_lookupPool = this; g_lookupHandle = handle; return g_block; }

// The member is private: reach it by its mangled name (a one-argument __thiscall is __fastcall(this, edx, arg)).
extern "C" bool __fastcall SimAllocate(void* self, int, CMovableResourceMemoryPool* pool);
#pragma comment(linker, "/alternatename:@SimAllocate@12=?AllocateArrays@CParticleSimulation@@AAE_NAAVCMovableResourceMemoryPool@@@Z")

static unsigned long Word(const unsigned char* raw, unsigned offset) { unsigned long v; memcpy(&v, raw + offset, 4); return v; }

int main()
{
    static unsigned char raw[0x80], pool[0x40];
    CParticleSimulation* s = reinterpret_cast<CParticleSimulation*>(raw);
    CMovableResourceMemoryPool& memoryPool = *reinterpret_cast<CMovableResourceMemoryPool*>(pool);

    // Failure: pool remembered, request made, arrays untouched, no lookup.
    memset(raw, 0xAB, sizeof(raw));
    unsigned long n = 3; memcpy(raw + 0x18, &n, 4);
    g_ok = false;
    CHECK(!SimAllocate(s, 0, &memoryPool));
    CHECK(Word(raw, 0x78) == (unsigned long)pool && g_allocs == 1 && g_allocPool == pool);
    CHECK(g_handleSlot == (long*)(raw + 0x74) && g_owner == raw && g_size == 3 * 0x44 && g_flags == 0 && g_lookups == 0);
    CHECK(Word(raw, 0x1C) == 0xABABABABu && Word(raw, 0x2C) == 0xABABABABu);

    // Success.
    memset(raw, 0, sizeof(raw)); n = 5; memcpy(raw + 0x18, &n, 4);
    g_ok = true;
    CHECK(SimAllocate(s, 0, &memoryPool));
    CHECK(g_size == 5 * 0x44 && Word(raw, 0x74) == 77 && g_lookups == 1 && g_lookupPool == pool && g_lookupHandle == 77);
    const unsigned long b = (unsigned long)g_block;
    CHECK(Word(raw, 0x1C) == b && Word(raw, 0x20) == b + 0x50 && Word(raw, 0x24) == b + 0xA0);
    CHECK(Word(raw, 0x28) == b + 0xF0 && Word(raw, 0x2C) == b + 0x140);
    CHECK(Word(raw, 0x78) == (unsigned long)pool && Word(raw, 0x18) == 5);

    if (g_fail) { printf("SIM_ALLOCATE_FAIL %d\n", g_fail); return 1; }
    printf("SIM_ALLOCATE_OK\n");
    return 0;
}
