// Behaviour fixture for CParticleProgram::AllocateArrays (provider CParticleProgram_AllocateArrays_00adf460.cpp). The
// program is raw storage at retail offsets; the fixture supplies the pool's AllocateResource and GetPointerFromHandle
// (recording; allocation can be told to fail). Checked: the pool is remembered in every case; a count whose byte size
// (16 per particle) overflows 32 bits is refused without asking the pool; otherwise count * 16 bytes are requested
// against the program's MemoryHandle, owned by the program, with no flags; on success NonSimPositions points at the block
// and true is returned; on failure NonSimPositions is untouched and false is returned.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"

static int g_fail, g_allocs, g_lookups;
static bool g_ok = true;
static const void* g_allocPool;
static long* g_handleSlot;
static const void* g_owner;
static unsigned long g_size, g_flags;
static long g_lookupHandle;
static unsigned char g_block[64];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

bool CMovableResourceMemoryPool::AllocateResource(long* handle, CMovableResource* owner, unsigned long size, unsigned long flags)
{
    ++g_allocs; g_allocPool = this; g_handleSlot = handle; g_owner = owner; g_size = size; g_flags = flags;
    if (!g_ok) return false;
    *handle = 31;
    return true;
}
void* CHandleBasedMemoryPool::GetPointerFromHandle(long handle) { ++g_lookups; g_lookupHandle = handle; return g_block; }

// The member is private: reach it by its mangled name (a two-argument __thiscall is __fastcall(this, edx, a, b)).
extern "C" bool __fastcall ProgramAllocate(void* self, int, unsigned long count, CMovableResourceMemoryPool* pool);
#pragma comment(linker, "/alternatename:@ProgramAllocate@16=?AllocateArrays@CParticleProgram@@AAE_NKAAVCMovableResourceMemoryPool@@@Z")

static unsigned long Word(const unsigned char* raw, unsigned offset) { unsigned long v; memcpy(&v, raw + offset, 4); return v; }

int main()
{
    static unsigned char raw[0x140], pool[0x40];
    CParticleProgram* program = reinterpret_cast<CParticleProgram*>(raw);
    CMovableResourceMemoryPool& memoryPool = *reinterpret_cast<CMovableResourceMemoryPool*>(pool);
    memset(raw, 0xAB, sizeof(raw));

    // Overflowing byte sizes: refused before the pool is asked (0x10000000 * 16 is exactly 2^32).
    CHECK(!ProgramAllocate(program, 0, 0x10000000u, &memoryPool));
    CHECK(!ProgramAllocate(program, 0, 0xFFFFFFFFu, &memoryPool));
    CHECK(g_allocs == 0 && Word(raw, 0x13C) == (unsigned long)pool);
    // The largest count that fits is passed on.
    g_ok = false;
    CHECK(!ProgramAllocate(program, 0, 0x0FFFFFFFu, &memoryPool));
    CHECK(g_allocs == 1 && g_size == 0xFFFFFFF0u && g_lookups == 0 && Word(raw, 0xC0) == 0xABABABABu);
    CHECK(g_handleSlot == (long*)(raw + 0x138) && g_owner == raw && g_flags == 0 && g_allocPool == pool);

    // Success.
    g_ok = true;
    CHECK(ProgramAllocate(program, 0, 3, &memoryPool));
    CHECK(g_size == 48 && Word(raw, 0x138) == 31 && g_lookups == 1 && g_lookupHandle == 31);
    CHECK(Word(raw, 0xC0) == (unsigned long)g_block && Word(raw, 0x13C) == (unsigned long)pool);

    if (g_fail) { printf("PROGRAM_ALLOCATE_FAIL %d\n", g_fail); return 1; }
    printf("PROGRAM_ALLOCATE_OK\n");
    return 0;
}
