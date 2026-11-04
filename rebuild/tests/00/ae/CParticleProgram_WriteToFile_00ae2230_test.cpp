// Behaviour fixture for CParticleProgram::WriteToFile (provider CParticleProgram_WriteToFile_00ae2230.cpp). The program
// is raw storage at retail offsets with hand-built STLport vectors; the stream is a fake object whose vtable slot 4
// (Write) records blocks. The stream writers, the constraint stream's GetLength / PeekData, the simulation's
// WriteToFile, WriteVectorArray and WriteGroups are supplied here (recording). Checked: the exact sequence - version
// 0x14D, constraint length + its bytes, the initial simulation (+0x08), each render array's count + block, NonSimCount + the non-simulated positions, texture coordinates and
// both index maps, AveragePatchSize, BezierEnable, the export count and each export's name and particle, then SimGroups
// (+0xFC) and NonSimGroups (+0x108).
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"
#include "fable_data_output_stream.h"

static int g_fail, g_n;
struct Op { char kind; unsigned long u; float f; const void* p; long size; } g_ops[48];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
static void Add(char k, unsigned long u, float f, const void* p, long s) { Op o = { k, u, f, p, s }; if (g_n < 48) g_ops[g_n] = o; ++g_n; }
static unsigned char g_constraints[20];

void CDataOutputStream::WriteULONG(unsigned long v) { Add('U', v, 0, 0, 0); }
void CDataOutputStream::WriteFloat(float v) { Add('F', 0, v, 0, 0); }
void CDataOutputStream::WriteEBOOL(bool v) { Add('B', v, 0, 0, 0); }
void CDataOutputStream::WriteNullTerminatedString(const CCharString& s) { Add('S', 0, 0, &s, 0); }
unsigned long CMemoryDataOutputStream::GetLength() const { return sizeof(g_constraints); }
const void* CMemoryDataOutputStream::PeekData() const { return g_constraints; }
void CParticleSimulation::WriteToFile(CDataOutputStream&) const { Add('I', 0, 0, this, 0); }
void FABLE_FASTCALL CParticleSimulation::WriteVectorArray(const C4DVector* v, unsigned long n, CDataOutputStream&) { Add('V', n, 0, v, 0); }
void FABLE_FASTCALL CParticleProgram::WriteGroups(CDataOutputStream&, const CArray<C3DGroup2>& g) { Add('G', 0, 0, &g, 0); }
static void __fastcall FakeWrite(void*, int, const void* data, long size) { Add('W', 0, 0, data, size); }
static void __fastcall NotReached(void*, int) { CHECK(!"other stream slot"); }

static unsigned char program[0x140];
static void Vec(unsigned o, void* b, unsigned n, unsigned elem)
{
    unsigned char* s = (unsigned char*)b; unsigned char* e = s + n * elem; memcpy(program + o, &s, 4); memcpy(program + o + 4, &e, 4); memcpy(program + o + 8, &e, 4);
}
#define EXPECT(i, k, cond) CHECK(g_ops[i].kind == k && (cond))

int main()
{
    static void* vtable[8] = { (void*)NotReached, (void*)NotReached, (void*)NotReached, (void*)NotReached, (void*)FakeWrite,
                               (void*)NotReached, (void*)NotReached, (void*)NotReached };
    static void* stream[4] = { vtable };
    static unsigned char quads[1 * 8], tris[2 * 6], verts[3 * 4], uv[2 * 8], pidx[4 * 4], vidx[1 * 4], nonSim[16];
    static unsigned long exports[2][2] = { { 0, 7 }, { 0, 9 } };
    memset(program, 0, sizeof(program));
    Vec(0x88, exports, 2, 8);
    Vec(0x98, tris, 2, 6);     // C3DTriangle2: three 16-bit indices
    Vec(0xA4, quads, 1, 8); Vec(0xB0, verts, 3, 4);
    unsigned long n = 1; memcpy(program + 0xBC, &n, 4); void* ns = nonSim; memcpy(program + 0xC0, &ns, 4);
    Vec(0xC4, uv, 2, 8); Vec(0xDC, pidx, 4, 4); Vec(0xE8, vidx, 1, 4);
    float patch = 0.375f; memcpy(program + 0xF8, &patch, 4); program[0xF4] = 1;
    reinterpret_cast<const CParticleProgram*>(program)->WriteToFile(*reinterpret_cast<CDataOutputStream*>(stream));

    CHECK(g_n == 27);
    EXPECT(0, 'U', g_ops[0].u == 0x14D);
    EXPECT(1, 'U', g_ops[1].u == 20);
    EXPECT(2, 'W', g_ops[2].p == g_constraints && g_ops[2].size == 20);
    EXPECT(3, 'I', g_ops[3].p == program + 0x08);
    EXPECT(4, 'U', g_ops[4].u == 2); EXPECT(5, 'W', g_ops[5].p == tris && g_ops[5].size == 12);
    EXPECT(6, 'U', g_ops[6].u == 1); EXPECT(7, 'W', g_ops[7].p == quads && g_ops[7].size == 8);
    EXPECT(8, 'U', g_ops[8].u == 3); EXPECT(9, 'W', g_ops[9].p == verts && g_ops[9].size == 12);
    EXPECT(10, 'U', g_ops[10].u == 1); EXPECT(11, 'V', g_ops[11].p == nonSim && g_ops[11].u == 1);
    EXPECT(12, 'U', g_ops[12].u == 2); EXPECT(13, 'W', g_ops[13].p == uv && g_ops[13].size == 16);
    EXPECT(14, 'U', g_ops[14].u == 4); EXPECT(15, 'W', g_ops[15].p == pidx && g_ops[15].size == 16);
    EXPECT(16, 'U', g_ops[16].u == 1); EXPECT(17, 'W', g_ops[17].p == vidx && g_ops[17].size == 4);
    EXPECT(18, 'F', g_ops[18].f == 0.375f); EXPECT(19, 'B', g_ops[19].u == 1);
    EXPECT(20, 'U', g_ops[20].u == 2);
    EXPECT(21, 'S', g_ops[21].p == &exports[0][0]); EXPECT(22, 'U', g_ops[22].u == 7);
    EXPECT(23, 'S', g_ops[23].p == &exports[1][0]); EXPECT(24, 'U', g_ops[24].u == 9);
    EXPECT(25, 'G', g_ops[25].p == program + 0xFC); EXPECT(26, 'G', g_ops[26].p == program + 0x108);

    if (g_fail) { printf("PROGRAM_WRITE_FAIL %d\n", g_fail); return 1; }
    printf("PROGRAM_WRITE_OK\n");
    return 0;
}
