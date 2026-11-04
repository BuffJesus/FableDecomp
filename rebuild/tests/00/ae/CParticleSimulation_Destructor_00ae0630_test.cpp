// Behaviour fixture for CParticleSimulation::~CParticleSimulation and CParticleProgram::~CParticleProgram (provider
// CParticleSimulation_Destructor_00ae0630.cpp), both called non-virtually on raw storage. The fixture supplies Clear (recording; it checks the render data is still held
// when it runs) and a render-data delete function. Checked: Clear runs first, exactly once; a shared render-data block
// only loses one reference; the last reference deletes the data through the block's delete function and frees the
// block; the render-data pointer and block are zeroed; the object ends with CMovableResource's vtable; no render data
// is handled without a block. ~CParticleProgram: CParticleProgram::Clear runs first, then (in reverse member order) the
// constraint stream's destructor runs on +0x114, every export name is destroyed, and the initial simulation (+0x08) is
// destroyed through the expanded ~CParticleSimulation (its Clear runs after the program's); the object ends with
// CMovableResource's vtable.
#include <stdio.h>
#include <string.h>
#include "fable_particle_simulation.h"
#include "fable_particle_program.h"

static int g_fail, g_clears, g_deletes;
static bool g_heldAtClear;
static void* g_deleted;
static unsigned char* g_sim;
static int g_step, g_programClears, g_programClearStep, g_streamDtors, g_streamStep, g_nameDtors, g_simClearStep;
static const void *g_programThis, *g_streamThis, *g_simClearThis;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void CParticleSimulation::Clear()
{
    ++g_clears; g_simClearStep = ++g_step; g_simClearThis = this;
    unsigned long info; memcpy(&info, g_sim + 8, 4); g_heldAtClear = info != 0;
}
// The provider's object emits CParticleSimulation's vtable; its other slots are not reached.
bool CMovableResource::CanRelocateData(long) const { CHECK(!"not reached"); return false; }
unsigned long CMovableResource::Verify() const { CHECK(!"not reached"); return 0; }
void CParticleSimulation::RelocateData(long, void*, void*, long) { CHECK(!"not reached"); }
void CParticleProgram::Clear() { ++g_programClears; g_programClearStep = ++g_step; g_programThis = this; }
// The stream destructor through /alternatename (defining it here would emit the stream's vtable); a no-argument
// __thiscall is a one-argument __fastcall.
extern "C" void __fastcall FixStreamDtor(void* self) { ++g_streamDtors; g_streamStep = ++g_step; g_streamThis = self; }
#pragma comment(linker, "/alternatename:??1CMemoryDataOutputStream@@UAE@XZ=@FixStreamDtor@4")
CCharString::~CCharString() { ++g_nameDtors; }
void CParticleProgram::RelocateData(long, void*, void*, long) { CHECK(!"not reached"); }
static void FABLE_FASTCALL DeleteRenderData(void* data) { ++g_deletes; g_deleted = data; }

// CMovableResource's vtable as the provider's object emits it.
extern "C" const void* const FixMovableVtable[];
#pragma comment(linker, "/alternatename:_FixMovableVtable=??_7CMovableResource@@6B@")

static unsigned long Word(const unsigned char* raw, unsigned offset) { unsigned long v; memcpy(&v, raw + offset, 4); return v; }

int main()
{
    static unsigned char sim[0x80], renderData[8];
    g_sim = sim;
    CParticleSimulation* s = reinterpret_cast<CParticleSimulation*>(sim);

    // Shared render data: one reference dropped, nothing deleted.
    CCPPointerInfo* shared = new CCPPointerInfo(DeleteRenderData, renderData);
    shared->RefCount = 2;
    memset(sim, 0x66, sizeof(sim));
    void* data = renderData; memcpy(sim + 4, &data, 4); memcpy(sim + 8, &shared, 4);
    s->CParticleSimulation::~CParticleSimulation();
    CHECK(g_clears == 1 && g_heldAtClear && shared->RefCount == 1 && g_deletes == 0);
    CHECK(Word(sim, 4) == 0 && Word(sim, 8) == 0 && Word(sim, 0) == (unsigned long)FixMovableVtable);

    // Last reference: data deleted through the block, block freed.
    memset(sim, 0x66, sizeof(sim));
    memcpy(sim + 4, &data, 4); memcpy(sim + 8, &shared, 4);
    s->CParticleSimulation::~CParticleSimulation();
    CHECK(g_clears == 2 && g_heldAtClear && g_deletes == 1 && g_deleted == renderData);
    CHECK(Word(sim, 4) == 0 && Word(sim, 8) == 0 && Word(sim, 0) == (unsigned long)FixMovableVtable);

    // No render data.
    memset(sim, 0x66, sizeof(sim)); memset(sim + 4, 0, 8);
    s->CParticleSimulation::~CParticleSimulation();
    CHECK(g_clears == 3 && !g_heldAtClear && g_deletes == 1 && Word(sim, 4) == 0 && Word(sim, 8) == 0);

    // ~CParticleProgram over empty member arrays (null storage) and two export names.
    {
        static unsigned char program[0x140]; static unsigned long names[2][2];
        memset(program, 0, sizeof(program));
        unsigned char* b = (unsigned char*)names; unsigned char* e = b + sizeof(names);
        memcpy(program + 0x88, &b, 4); memcpy(program + 0x8C, &e, 4); memcpy(program + 0x90, &e, 4);
        g_sim = program + 0x08; g_step = 0; int clearsBefore = g_clears;
        CParticleProgram* pr = reinterpret_cast<CParticleProgram*>(program);
        pr->CParticleProgram::~CParticleProgram();
        CHECK(g_programClears == 1 && g_programThis == program && g_programClearStep == 1);
        CHECK(g_streamDtors == 1 && g_streamThis == program + 0x114 && g_streamStep > g_programClearStep);
        CHECK(g_nameDtors == 2);
        CHECK(g_clears == clearsBefore + 1 && g_simClearThis == program + 0x08 && g_simClearStep > g_streamStep);
        CHECK(Word(program, 0x08) == (unsigned long)FixMovableVtable && Word(program, 0) == (unsigned long)FixMovableVtable);
    }

    if (g_fail) { printf("SIMULATION_DESTRUCTOR_FAIL %d\n", g_fail); return 1; }
    printf("SIMULATION_DESTRUCTOR_OK\n");
    return 0;
}
