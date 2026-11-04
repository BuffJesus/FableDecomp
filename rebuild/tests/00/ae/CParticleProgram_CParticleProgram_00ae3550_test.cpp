// Behaviour fixture for CParticleProgram::CParticleProgram and the CParticleSimulation constructor it expands (retail
// 0x00AE0600, defined first in the provider CParticleProgram_CParticleProgram_00ae3550.cpp). It
// constructs a program in place and checks the retail-visible state: both vtables (program 0x0129F0D8 and its embedded
// simulation 0x0129F0C4 are the provider's own), every array empty, the export map clean, the simulation and the program
// both unallocated with no pool, the simulation's Clear run before the program's Clear (each exactly once, on the right
// objects), and the constraint stream constructed at +0x114. Clear bodies, the stream constructor and the virtuals the
// vtables reference are supplied here (recording), as they live in other sources.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_particle_program.h"

static int g_fail, g_step, g_simClearStep, g_progClearStep, g_streams;
static const void* g_simClearThis;
static const void* g_progClearThis;
static const void* g_streamThis;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void CParticleSimulation::Clear() { g_simClearThis = this; g_simClearStep = ++g_step; }
void CParticleProgram::Clear() { g_progClearThis = this; g_progClearStep = ++g_step; }
void CParticleSimulation::RelocateData(long, void*, void*, long) {}
// The destructors live in CParticleSimulation_Destructor_00ae0630.cpp; the vtables reference them, nothing calls them.
CParticleSimulation::~CParticleSimulation() { CHECK(!"not reached"); }
CParticleProgram::~CParticleProgram() { CHECK(!"not reached"); }
void CParticleProgram::RelocateData(long, void*, void*, long) {}
bool CMovableResource::CanRelocateData(long) const { return false; }
unsigned long CMovableResource::Verify() const { return 0; }

// The stream constructor/destructor and the name destructor by mangled name (defining them here would emit the
// stream's vtable): a no-argument __thiscall is call-compatible with a one-argument __fastcall.
extern "C" void* __fastcall FakeStreamCtor(void* self) { ++g_streams; g_streamThis = self; return self; }
extern "C" void __fastcall FakeStreamDtor(void*) { --g_streams; }
extern "C" void __fastcall FakeNameDtor(void*) {}
#pragma comment(linker, "/alternatename:??0CMemoryDataOutputStream@@QAE@XZ=@FakeStreamCtor@4")
#pragma comment(linker, "/alternatename:??1CMemoryDataOutputStream@@UAE@XZ=@FakeStreamDtor@4")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@FakeNameDtor@4")

static bool EmptyVector(const unsigned char* raw, unsigned offset)
{
    static const unsigned char zero[12] = { 0 };
    return memcmp(raw + offset, zero, 12) == 0;
}

int main()
{
    static unsigned char raw[sizeof(CParticleProgram)];
    memset(raw, 0xCD, sizeof(raw));
    CParticleProgram* program = new (raw) CParticleProgram;
    CHECK(static_cast<void*>(program) == raw);

    // Clears: the simulation's first (inside its own construction), then the program's.
    CHECK(g_simClearStep == 1 && g_simClearThis == raw + 0x08);
    CHECK(g_progClearStep == 2 && g_progClearThis == raw);
    CHECK(g_streams == 1 && g_streamThis == raw + 0x114);

    unsigned long w;
    memcpy(&w, raw + 0x74 + 0x08, 4); CHECK(w == 0xFFFFFFFFu);    // simulation MemoryHandle
    memcpy(&w, raw + 0x78 + 0x08, 4); CHECK(w == 0);              // simulation MemoryPool
    CHECK(memcmp(raw + 0x04 + 0x08, "\0\0\0\0\0\0\0\0", 8) == 0); // simulation RenderData
    memcpy(&w, raw + 0x138, 4); CHECK(w == 0xFFFFFFFFu);          // program MemoryHandle
    memcpy(&w, raw + 0x13C, 4); CHECK(w == 0);                    // program MemoryPool
    const unsigned arrays[] = { 0x98, 0xA4, 0xB0, 0xC4, 0xD0, 0xDC, 0xE8, 0xFC, 0x108, 0x12C };
    for (unsigned i = 0; i < sizeof(arrays) / sizeof(arrays[0]); ++i) CHECK(EmptyVector(raw, arrays[i]));
    CHECK(EmptyVector(raw, 0x88) && raw[0x95] == 0);              // export map: empty and not dirty
    unsigned long vtable, simVtable;
    memcpy(&vtable, raw, 4); memcpy(&simVtable, raw + 0x08, 4);
    CHECK(vtable != 0xCDCDCDCDu && simVtable != 0xCDCDCDCDu && vtable != simVtable);

    if (g_fail) { printf("PARTICLE_PROGRAM_CTOR_FAIL %d\n", g_fail); return 1; }
    printf("PARTICLE_PROGRAM_CTOR_OK\n");
    return 0;
}
