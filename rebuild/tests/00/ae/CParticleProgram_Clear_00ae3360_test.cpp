// Behaviour fixture for CParticleProgram::Clear (provider CParticleProgram_Clear_00ae3360.cpp). The program is raw storage
// laid out like retail with every array pointing at fixture data; the fixture supplies the collaborators Clear reaches
// (CCharString destructor/assignment, the constraint stream's Clear, the pool's FreeResource, blend-vector assignment).
// Checked: ExportParticles, RenderTris/Quads/Vertices, SimGroups/NonSimGroups and both vertex maps end up empty (each
// export name destroyed once); BezierEnable, AveragePatchSize, NonSimCount and NonSimPositions are zeroed; the
// constraint stream is cleared; the particle memory is freed once and marked unallocated (and not again on a second
// Clear); InitialSimulation, IndexedTextureCoords, TempVertexPositions and LoopStack are untouched.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"

static int g_fail, g_nameDtors, g_streamClears, g_frees;
static const void* g_streamThis;
static const void* g_freePool;
static long g_freeHandle;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

CCharString::~CCharString() { ++g_nameDtors; }
CCharString& CCharString::operator=(const CCharString&) { CHECK(!"no name is moved when the whole list is erased"); return *this; }
void CMemoryDataOutputStream::Clear() { ++g_streamClears; g_streamThis = this; }
void CMovableResourceMemoryPool::FreeResource(long handle) { ++g_frees; g_freePool = this; g_freeHandle = handle; }
namespace _STL
{
template <> vector<C3DVertexBlend2, allocator<C3DVertexBlend2> >& vector<C3DVertexBlend2, allocator<C3DVertexBlend2> >::operator=(const vector<C3DVertexBlend2, allocator<C3DVertexBlend2> >&)
{
    CHECK(!"no group is moved when the whole list is erased"); return *this;
}
}

static unsigned char raw[0x140], before[0x140];
static void SetVector(unsigned offset, void* begin, unsigned count, unsigned elementSize)
{
    unsigned char* b = (unsigned char*)begin; unsigned char* e = b + count * elementSize;
    memcpy(raw + offset, &b, 4); memcpy(raw + offset + 4, &e, 4); memcpy(raw + offset + 8, &e, 4);
}
static bool Empty(unsigned offset) { return memcmp(raw + offset, raw + offset + 4, 4) == 0; }

int main()
{
    static unsigned long names[3][2];       // pair<CCharString, unsigned long>: name storage left zero
    static unsigned char tris[2 * 12], quads[3 * 8], verts[5 * 4], texcoords[2 * 8], temps[2 * 16], loops[2 * 8];
    static unsigned long groups[2][4], nonSimGroups[1][4];   // C3DGroup2 with empty blend vectors
    static long particles[4], vertices[4];
    for (int i = 0; i < 0x140; ++i) raw[i] = (unsigned char)(0x11 + i);
    SetVector(0x88, names, 3, 8); raw[0x95] = 1;
    SetVector(0x98, tris, 2, 12); SetVector(0xA4, quads, 3, 8); SetVector(0xB0, verts, 5, 4);
    SetVector(0xC4, texcoords, 2, 8); SetVector(0xD0, temps, 2, 16);
    SetVector(0xDC, particles, 4, 4); SetVector(0xE8, vertices, 4, 4);
    SetVector(0xFC, groups, 2, 16); SetVector(0x108, nonSimGroups, 1, 16);
    SetVector(0x12C, loops, 2, 8);
    raw[0xF4] = 1;
    long handle = 9; memcpy(raw + 0x138, &handle, 4);
    static unsigned char pool[16]; void* p = pool; memcpy(raw + 0x13C, &p, 4);
    memcpy(before, raw, sizeof(raw));
    CParticleProgram* program = reinterpret_cast<CParticleProgram*>(raw);

    program->Clear();
    CHECK(Empty(0x88) && g_nameDtors == 3);
    CHECK(Empty(0x98) && Empty(0xA4) && Empty(0xB0));
    CHECK(Empty(0xDC) && Empty(0xE8) && Empty(0xFC) && Empty(0x108));
    unsigned long zero = 0; float zf = 0.0f;
    CHECK(raw[0xF4] == 0 && memcmp(raw + 0xF8, &zf, 4) == 0);
    CHECK(memcmp(raw + 0xBC, &zero, 4) == 0 && memcmp(raw + 0xC0, &zero, 4) == 0);
    CHECK(g_streamClears == 1 && g_streamThis == raw + 0x114);
    CHECK(g_frees == 1 && g_freePool == pool && g_freeHandle == 9);
    long after; memcpy(&after, raw + 0x138, 4); CHECK(after == -1);
    CHECK(memcmp(raw + 0x08, before + 0x08, 0x80) == 0);                       // initial simulation
    CHECK(memcmp(raw + 0xC4, before + 0xC4, 0x18) == 0);                       // texture coords, temp positions
    CHECK(memcmp(raw + 0x12C, before + 0x12C, 0x0C) == 0);                     // loop stack
    CHECK(memcmp(raw + 0x13C, before + 0x13C, 4) == 0);                        // pool pointer kept

    program->Clear();
    CHECK(g_frees == 1 && g_nameDtors == 3 && g_streamClears == 2);

    if (g_fail) { printf("PARTICLE_PROGRAM_CLEAR_FAIL %d\n", g_fail); return 1; }
    printf("PARTICLE_PROGRAM_CLEAR_OK\n");
    return 0;
}
