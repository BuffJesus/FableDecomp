// Behaviour fixture for CParticleSimulation::ReadFromFile (provider CParticleSimulation_ReadFromFile_00ae1bc0.cpp). The stream
// is raw storage over an in-memory buffer (StreamPos +0x04, CurrentSourcePtr +0x0C, bytes left +0x14) so every read takes
// the inline in-window path; the fixture supplies AllocateArrays (static arrays; can be told to fail) and ReadVectorArray
// (three floats per particle from the same stream, W from the argument). Checked: the timestep settings and count are
// read first; a failed allocation stops the read and returns false; otherwise positions are read and copied into the
// last and saved positions, the alphas are read raw, then gravity, wind, dragging (two flags + strength), acceleration
// flag and damping; the bounding box is marked stale; the stream ends exactly after the data; true is returned.
#include <stdio.h>
#include <string.h>
#include "fable_particle_simulation.h"
#include "fable_data_input_stream.h"
#include "fable_4d_vector.h"

static int g_fail, g_overflows;
static bool g_allocateOk = true;
static float g_w = -1.0f;
static C4DVector g_pos[4], g_last[4], g_saved[4], g_acc[4];
static float g_alpha[4];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

bool CParticleSimulation::AllocateArrays(CMovableResourceMemoryPool&)
{
    if (!g_allocateOk) return false;
    Positions = g_pos; LastPositions = g_last; SavedPositions = g_saved; Accelerations = g_acc; SimulationAlphas = g_alpha;
    return true;
}
void FABLE_FASTCALL CParticleSimulation::ReadVectorArray(C4DVector* vectors, unsigned long count, CDataInputStream& stream, float w)
{
    g_w = w;
    for (unsigned long i = 0; i < count; ++i)
    {
        stream.Read(&vectors[i].X, 4); stream.Read(&vectors[i].Y, 4); stream.Read(&vectors[i].Z, 4); vectors[i].W = w;
    }
}
void CDataInputStream::ReadWithSrcChunkOverflow(void*, long) { ++g_overflows; }

static unsigned char g_stream[0x40];
static void Attach(const void* data, long size)
{
    memset(g_stream, 0, sizeof(g_stream));
    const unsigned char* src = (const unsigned char*)data;
    memcpy(g_stream + 0x0C, &src, 4); memcpy(g_stream + 0x14, &size, 4);
}

int main()
{
#pragma pack(push, 1)
    struct Packed
    {
        float timestep; char changed; float multiplier; unsigned long size;
        float pos[2][3]; float alpha[2];
        float gravity, wind; char dragging, rotational; float dragStrength; char acceleration; float damping;
    } d = { 0.05f, 1, 2.0f, 2, { { 1, 2, 3 }, { 4, 5, 6 } }, { 0.25f, 0.75f }, -9.8f, 3.5f, 0, 1, 0.6f, 1, 0.9f };
#pragma pack(pop)
    static unsigned char raw[0x80];
    static unsigned char pool[8];
    CParticleSimulation* s = reinterpret_cast<CParticleSimulation*>(raw);
    CDataInputStream& stream = *reinterpret_cast<CDataInputStream*>(g_stream);

    // Allocation failure: stops after the count, returns false.
    memset(raw, 0, sizeof(raw));
    Attach(&d, sizeof(d)); g_allocateOk = false;
    CHECK(!s->ReadFromFile(stream, *reinterpret_cast<CMovableResourceMemoryPool*>(pool)));
    CHECK(s->Timestep == 0.05f && s->TimestepChanged && s->TimestepMultiplier == 2.0f && s->Size == 2);
    unsigned long pos; memcpy(&pos, g_stream + 4, 4); CHECK(pos == 13);
    CHECK(s->GravityStrength == 0.0f && g_w == -1.0f);

    // Full read.
    memset(raw, 0, sizeof(raw)); raw[0x70] = 1;
    Attach(&d, sizeof(d)); g_allocateOk = true;
    CHECK(s->ReadFromFile(stream, *reinterpret_cast<CMovableResourceMemoryPool*>(pool)));
    CHECK(g_w == 0.0f && g_pos[1].X == 4 && g_pos[1].Z == 6 && g_pos[0].W == 0.0f);
    CHECK(memcmp(g_last, g_pos, 2 * sizeof(C4DVector)) == 0 && memcmp(g_saved, g_pos, 2 * sizeof(C4DVector)) == 0);
    CHECK(g_alpha[0] == 0.25f && g_alpha[1] == 0.75f);
    CHECK(s->GravityStrength == -9.8f && s->WindStrength == 3.5f && !s->DraggingEnable && s->DraggingRotational);
    CHECK(s->DraggingStrength == 0.6f && s->AccelerationEnable && s->GlobalDamping == 0.9f && !s->BoundingBoxValid);
    memcpy(&pos, g_stream + 4, 4); long left; memcpy(&left, g_stream + 0x14, 4);
    CHECK(pos == sizeof(d) && left == 0 && g_overflows == 0);

    if (g_fail) { printf("SIMULATION_READ_FAIL %d\n", g_fail); return 1; }
    printf("SIMULATION_READ_OK\n");
    return 0;
}
