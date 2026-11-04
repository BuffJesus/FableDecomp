// Behaviour fixture for CParticleSimulation::WriteToFile (provider CParticleSimulation_WriteToFile_00ae2050.cpp). The
// simulation is raw storage at retail offsets; the stream is a fake object whose vtable slot 4 (Write) records blocks.
// WriteFloat / WriteULONG / WriteEBOOL and the protected static WriteVectorArray are supplied here (recording). Checked:
// the exact write sequence and values - Timestep (float), TimestepChanged (as a ULONG 0/1), TimestepMultiplier, Size,
// the Positions array (Size vectors, through WriteVectorArray on the same stream), the alphas as one Size * 4 byte block,
// GravityStrength, WindStrength, DraggingEnable, DraggingRotational, DraggingStrength, AccelerationEnable,
// GlobalDamping - and nothing else (MaximumAdjustment, the clock and the bounding box are not written).
#include <stdio.h>
#include <string.h>
#include "fable_particle_simulation.h"
#include "fable_data_output_stream.h"

static int g_fail, g_n;
struct Op { char kind; unsigned long u; float f; const void* p; long size; } g_ops[32];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
static void Add(char k, unsigned long u, float f, const void* p, long s) { Op o = { k, u, f, p, s }; if (g_n < 32) g_ops[g_n] = o; ++g_n; }

void CDataOutputStream::WriteULONG(unsigned long v) { Add('U', v, 0, this, 0); }
void CDataOutputStream::WriteFloat(float v) { Add('F', 0, v, this, 0); }
void CDataOutputStream::WriteEBOOL(bool v) { Add('B', v, 0, this, 0); }
void FABLE_FASTCALL CParticleSimulation::WriteVectorArray(const C4DVector* v, unsigned long count, CDataOutputStream& s)
{
    Add('V', count, 0, v, (long)&s);
}
static void __fastcall FakeWrite(void*, int, const void* data, long size) { Add('W', 0, 0, data, size); }
static void __fastcall NotReached(void*, int) { CHECK(!"other stream slot"); }
static void PutF(unsigned char* r, unsigned o, float v) { memcpy(r + o, &v, 4); }
static void PutU(unsigned char* r, unsigned o, unsigned long v) { memcpy(r + o, &v, 4); }

int main()
{
    static void* vtable[8] = { (void*)NotReached, (void*)NotReached, (void*)NotReached, (void*)NotReached, (void*)FakeWrite,
                               (void*)NotReached, (void*)NotReached, (void*)NotReached };
    static void* stream[4] = { vtable };
    static unsigned char sim[0x80], positions[48], alphas[12];
    memset(sim, 0, sizeof(sim));
    PutF(sim, 0x0C, 0.0625f); sim[0x10] = 1; PutF(sim, 0x14, 1.5f); PutU(sim, 0x18, 3);
    void* p = positions; memcpy(sim + 0x1C, &p, 4); void* a = alphas; memcpy(sim + 0x2C, &a, 4);
    PutF(sim, 0x30, 99.0f); PutF(sim, 0x34, 2.0f); PutF(sim, 0x38, 3.0f); sim[0x3C] = 1; sim[0x3D] = 0; PutF(sim, 0x40, 0.75f);
    sim[0x44] = 1; PutF(sim, 0x48, 0.125f);
    const CParticleSimulation& s = *reinterpret_cast<const CParticleSimulation*>(sim);
    s.WriteToFile(*reinterpret_cast<CDataOutputStream*>(stream));

    CHECK(g_n == 13);
    CHECK(g_ops[0].kind == 'F' && g_ops[0].f == 0.0625f && g_ops[0].p == stream);
    CHECK(g_ops[1].kind == 'U' && g_ops[1].u == 1);
    CHECK(g_ops[2].kind == 'F' && g_ops[2].f == 1.5f);
    CHECK(g_ops[3].kind == 'U' && g_ops[3].u == 3);
    CHECK(g_ops[4].kind == 'V' && g_ops[4].u == 3 && g_ops[4].p == positions && g_ops[4].size == (long)stream);
    CHECK(g_ops[5].kind == 'W' && g_ops[5].p == alphas && g_ops[5].size == 12);
    CHECK(g_ops[6].kind == 'F' && g_ops[6].f == 2.0f && g_ops[7].kind == 'F' && g_ops[7].f == 3.0f);
    CHECK(g_ops[8].kind == 'B' && g_ops[8].u == 1 && g_ops[9].kind == 'B' && g_ops[9].u == 0);
    CHECK(g_ops[10].kind == 'F' && g_ops[10].f == 0.75f);
    CHECK(g_ops[11].kind == 'B' && g_ops[11].u == 1);
    CHECK(g_ops[12].kind == 'F' && g_ops[12].f == 0.125f);

    if (g_fail) { printf("SIMULATION_WRITE_FAIL %d\n", g_fail); return 1; }
    printf("SIMULATION_WRITE_OK\n");
    return 0;
}
