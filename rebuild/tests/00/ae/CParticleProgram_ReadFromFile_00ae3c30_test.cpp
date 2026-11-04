// Behaviour fixture for CParticleProgram::ReadFromFile (provider CParticleProgram_ReadFromFile_00ae3c30.cpp). The program is
// raw zeroed storage at retail offsets; the stream is raw storage over an in-memory buffer (vtable +0x00, StreamPos +0x04,
// CurrentSourcePtr +0x0C, bytes left +0x14) so every scalar read takes the inline in-window path, with a fake vtable whose
// slot 6 is ReadNullTerminatedString. Collaborators that live in other units are supplied here, recording: the constraint
// stream's Clear and CopyTo, the initial simulation's ReadFromFile (reads its particle count), AllocateArrays, the
// simulation's ReadVectorArray, the vector resizes and the export list's reallocating insert (raw buffers), CCharString
// copy/destroy (a 4-byte name pointer), the export sort (by name) and ReadGroups.
// Checked: the version word is skipped; the constraint stream is cleared and then receives the stored byte count; a
// failing simulation read or allocation returns false with the rest unread; otherwise every array is sized and filled
// from the stream in order, NonSimCount is stored and the allocation asks for simulated + non-simulated particles, the
// non-simulated positions are read with W = 0, the patch size and Bezier flag are read, the export list is resized to the
// stored count, the entries are appended after the defaults (in place while capacity lasts, then through the
// reallocating insert), each name copied once and every temporary destroyed, the list is sorted once and marked clean,
// both group lists are read (simulated first), the stream ends exactly after the data, and true is returned.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_particle_program.h"
#include "fable_data_input_stream.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

typedef _STL::pair<CCharString, unsigned long> Export;
static int g_step, g_clearStep, g_copyStep, g_overflows, g_insertOverflows, g_sorts, g_copies, g_destroys;
static unsigned long g_copySize, g_allocCount;
static const void* g_clearThis;
static const void* g_copyTarget;
static const void* g_groups[4];
static int g_groupCount;
static bool g_simOk = true, g_allocOk = true;
static float g_w = -1.0f;
static C4DVector g_nonSim[4];

// --- stream -------------------------------------------------------------------------------------------------------
static unsigned char g_stream[0x18];
static void Skip(unsigned long n)
{
    unsigned long pos; const unsigned char* src; long left;
    memcpy(&pos, g_stream + 4, 4); memcpy(&src, g_stream + 0x0C, 4); memcpy(&left, g_stream + 0x14, 4);
    pos += n; src += n; left -= (long)n;
    memcpy(g_stream + 4, &pos, 4); memcpy(g_stream + 0x0C, &src, 4); memcpy(g_stream + 0x14, &left, 4);
}
static const unsigned char* Source() { const unsigned char* s; memcpy(&s, g_stream + 0x0C, 4); return s; }
void CDataInputStream::ReadWithSrcChunkOverflow(void*, long) { ++g_overflows; }
void CDataInputStream::CopyTo(CDataOutputStream& target, unsigned long size)
{
    g_copyStep = ++g_step; g_copyTarget = &target; g_copySize = size; Skip(size);
}
// Slot 6: CCharString ReadNullTerminatedString() (thiscall, hidden return pointer = the one stack argument).
static void* __fastcall FakeReadString(void*, int, void* result)
{
    const char* text = (const char*)Source();
    memcpy(result, &text, 4);                 // the name points into the stream buffer
    Skip((unsigned long)strlen(text) + 1);
    return result;
}
static void* g_vtable[8] = { 0, 0, 0, 0, 0, 0, (void*)&FakeReadString, 0 };
static void Attach(const void* data, long size)
{
    memset(g_stream, 0, sizeof(g_stream));
    void** vt = g_vtable; memcpy(g_stream, &vt, 4);
    const unsigned char* src = (const unsigned char*)data;
    memcpy(g_stream + 0x0C, &src, 4); memcpy(g_stream + 0x14, &size, 4);
}

// --- collaborators ------------------------------------------------------------------------------------------------
void CMemoryDataOutputStream::Clear() { g_clearStep = ++g_step; g_clearThis = this; }
bool CParticleSimulation::ReadFromFile(CDataInputStream& stream, CMovableResourceMemoryPool&)
{
    Size = stream.ReadULONG();
    return g_simOk;
}
bool CParticleProgram::AllocateArrays(unsigned long count, CMovableResourceMemoryPool&)
{
    g_allocCount = count;
    if (!g_allocOk) return false;
    NonSimPositions = g_nonSim;
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
void FABLE_FASTCALL CParticleProgram::ReadGroups(CDataInputStream&, CArray<C3DGroup2>& groups) { g_groups[g_groupCount++ & 3] = &groups; }
CCharString::CCharString(const CCharString& other) { ++g_copies; memcpy(this, &other, 4); }
CCharString::~CCharString() { ++g_destroys; }

// Raw vector storage: begin / finish / end of storage, element size known per instantiation.
static void SetRaw(void* v, unsigned char* begin, unsigned long count, unsigned long capacity, unsigned long size)
{
    unsigned char* f = begin + count * size; unsigned char* e = begin + capacity * size;
    memcpy(v, &begin, 4); memcpy((char*)v + 4, &f, 4); memcpy((char*)v + 8, &e, 4);
}
static void Grow(void* v, unsigned long n, const void* fill, unsigned long size, unsigned long extra)
{
    unsigned char* b = (unsigned char*)calloc(n + extra + 1, size);
    for (unsigned long i = 0; i < n; ++i) if (fill) memcpy(b + i * size, fill, size);
    SetRaw(v, b, n, n + extra, size);
}
namespace _STL
{
template <> void vector<C3DQuad2, allocator<C3DQuad2> >::resize(size_t n) { Grow(this, n, 0, 8, 0); }
template <> void vector<C2DVector, allocator<C2DVector> >::resize(size_t n) { Grow(this, n, 0, 8, 0); }
// One spare slot: the first appended entry is constructed in place, the rest go through the reallocating insert.
template <> void vector<Export, allocator<Export> >::resize(size_t n) { Grow(this, n, 0, 8, 1); }
template <> void vector<C3DTriangle2, allocator<C3DTriangle2> >::resize(size_t n, const C3DTriangle2& x) { Grow(this, n, &x, 6, 0); }
template <> void vector<C3DClothRenderVertex, allocator<C3DClothRenderVertex> >::resize(size_t n, const C3DClothRenderVertex& x) { Grow(this, n, &x, 4, 0); }
template <> void vector<long, allocator<long> >::resize(size_t n, const long& x) { Grow(this, n, &x, 4, 0); }
template <> void vector<Export, allocator<Export> >::_M_insert_overflow(Export* position, const Export& x, const __false_type&, size_t fill, bool atEnd)
{
    ++g_insertOverflows;
    CHECK(position == _M_finish && fill == 1 && atEnd);
    size_t count = _M_finish - _M_start;
    unsigned char* b = (unsigned char*)calloc(count + 2, 8);
    memcpy(b, _M_start, count * 8);
    new (b + count * 8) CCharString(x.first);
    memcpy(b + count * 8 + 4, &x.second, 4);
    SetRaw(this, b, count + 1, count + 1, 8);
}
template <> void FABLE_FASTCALL sort<Export*, CKeyPairCompareLess<CCharString, unsigned long> >(Export* first, Export* last, CKeyPairCompareLess<CCharString, unsigned long>)
{
    ++g_sorts;
    for (Export* i = first + 1; i < last; ++i)       // insertion sort by name (a null name sorts first)
        for (Export* j = i; j > first; --j)
        {
            const char* a; const char* b; memcpy(&a, &(j - 1)->first, 4); memcpy(&b, &j->first, 4);
            if (strcmp(a ? a : "", b ? b : "") <= 0) break;
            unsigned char t[8]; memcpy(t, j - 1, 8); memcpy(j - 1, j, 8); memcpy(j, t, 8);
        }
}
}

// The one-argument resize is defined inside the class, so its specializations count as inline: referencing them makes
// this unit emit them for the provider.
typedef void (_STL::vector<C3DQuad2>::*QuadResize)(size_t);
typedef void (_STL::vector<C2DVector>::*UvResize)(size_t);
typedef void (_STL::vector<Export>::*ExportResize)(size_t);
volatile QuadResize g_keepQuad = &_STL::vector<C3DQuad2>::resize;
volatile UvResize g_keepUv = &_STL::vector<C2DVector>::resize;
volatile ExportResize g_keepExport = &_STL::vector<Export>::resize;

// --- data ---------------------------------------------------------------------------------------------------------
static unsigned char g_data[512];
static unsigned long g_len;
static void PutU(unsigned long v) { memcpy(g_data + g_len, &v, 4); g_len += 4; }
static void PutF(float v) { memcpy(g_data + g_len, &v, 4); g_len += 4; }
static void PutB(char v) { g_data[g_len++] = (unsigned char)v; }
static void PutS(const char* s) { memcpy(g_data + g_len, s, strlen(s) + 1); g_len += (unsigned long)strlen(s) + 1; }
static void PutRaw(const void* p, unsigned long n) { memcpy(g_data + g_len, p, n); g_len += n; }
static unsigned long Count(const unsigned char* raw, unsigned offset, unsigned size)
{
    unsigned char* b; unsigned char* e; memcpy(&b, raw + offset, 4); memcpy(&e, raw + offset + 4, 4); return (unsigned long)(e - b) / size;
}
static const unsigned char* Begin(const unsigned char* raw, unsigned offset) { unsigned char* b; memcpy(&b, raw + offset, 4); return b; }

int main()
{
    static unsigned char raw[0x140];
    static unsigned char pool[8];
    CParticleProgram* program = reinterpret_cast<CParticleProgram*>(raw);
    CMovableResourceMemoryPool& memoryPool = *reinterpret_cast<CMovableResourceMemoryPool*>(pool);
    CDataInputStream& stream = *reinterpret_cast<CDataInputStream*>(g_stream);

    // Stream: version, 3 constraint bytes, simulation (count 2), 2 tris, 1 quad, 3 render vertices, 2 non-sim, 2 texture
    // coordinates, 3 particle indices, 2 vertex indices, patch size, Bezier, 3 exports, (groups are recorded only).
    const unsigned short tris[6] = { 1, 2, 3, 4, 5, 6 };
    const unsigned short quad[4] = { 7, 8, 9, 10 };
    const unsigned short verts[6] = { 11, 12, 13, 14, 15, 16 };
    const float uv[4] = { 0.25f, 0.5f, 0.75f, 1.0f };
    const long particles[3] = { 0, -1, 1 };
    const long vertices[2] = { -1, 2 };
    PutU(0xFEEDu); PutU(3); PutB(1); PutB(2); PutB(3);
    PutU(2);
    PutU(2); PutRaw(tris, sizeof(tris));
    PutU(1); PutRaw(quad, sizeof(quad));
    PutU(3); PutRaw(verts, sizeof(verts));
    PutU(2); PutF(1); PutF(2); PutF(3); PutF(4); PutF(5); PutF(6);
    PutU(2); PutRaw(uv, sizeof(uv));
    PutU(3); PutRaw(particles, sizeof(particles));
    PutU(2); PutRaw(vertices, sizeof(vertices));
    PutF(0.375f); PutB(1);
    PutU(3); PutS("tip"); PutU(30); PutS("hem"); PutU(10); PutS("cuff"); PutU(20);
    const unsigned long full = g_len;

    // A failing simulation read stops after it.
    memset(raw, 0, sizeof(raw)); Attach(g_data, (long)full); g_simOk = false;
    CHECK(!program->ReadFromFile(stream, memoryPool));
    CHECK(g_clearStep == 1 && g_clearThis == raw + 0x114 && g_copyStep == 2 && g_copyTarget == raw + 0x114 && g_copySize == 3);
    unsigned long pos; memcpy(&pos, g_stream + 4, 4); CHECK(pos == 4 + 4 + 3 + 4);
    CHECK(Count(raw, 0x98, 6) == 0 && g_groupCount == 0);

    // A failing allocation stops after the render arrays and the non-simulated count.
    memset(raw, 0, sizeof(raw)); Attach(g_data, (long)full); g_simOk = true; g_allocOk = false;
    CHECK(!program->ReadFromFile(stream, memoryPool));
    CHECK(g_allocCount == 4 && Count(raw, 0xB0, 4) == 3 && Count(raw, 0xC4, 8) == 0 && g_w == -1.0f);
    unsigned long nonSim; memcpy(&nonSim, raw + 0xBC, 4); CHECK(nonSim == 2);

    // Full read.
    memset(raw, 0, sizeof(raw)); Attach(g_data, (long)full); g_allocOk = true; g_groupCount = 0;
    g_copies = g_destroys = 0;
    raw[0x88 + 0x0D] = 0;
    CHECK(program->ReadFromFile(stream, memoryPool));
    CHECK(Count(raw, 0x98, 6) == 2 && memcmp(Begin(raw, 0x98), tris, sizeof(tris)) == 0);
    CHECK(Count(raw, 0xA4, 8) == 1 && memcmp(Begin(raw, 0xA4), quad, sizeof(quad)) == 0);
    CHECK(Count(raw, 0xB0, 4) == 3 && memcmp(Begin(raw, 0xB0), verts, sizeof(verts)) == 0);
    CHECK(g_allocCount == 4 && g_w == 0.0f && g_nonSim[1].X == 4 && g_nonSim[1].Z == 6 && g_nonSim[0].W == 0.0f);
    CHECK(Count(raw, 0xC4, 8) == 2 && memcmp(Begin(raw, 0xC4), uv, sizeof(uv)) == 0);
    CHECK(Count(raw, 0xDC, 4) == 3 && memcmp(Begin(raw, 0xDC), particles, sizeof(particles)) == 0);
    CHECK(Count(raw, 0xE8, 4) == 2 && memcmp(Begin(raw, 0xE8), vertices, sizeof(vertices)) == 0);
    float patch; memcpy(&patch, raw + 0xF8, 4); CHECK(patch == 0.375f && raw[0xF4] == 1);
    // Exports: 3 default entries then the 3 stored ones, sorted by name (the defaults' null names first).
    CHECK(Count(raw, 0x88, 8) == 6 && g_insertOverflows == 2 && g_sorts == 1 && raw[0x88 + 0x0D] == 0);
    const unsigned char* e = Begin(raw, 0x88);
    const char* names[6]; unsigned long ids[6];
    for (int i = 0; i < 6; ++i) { memcpy(&names[i], e + 8 * i, 4); memcpy(&ids[i], e + 8 * i + 4, 4); }
    CHECK(!names[0] && !names[1] && !names[2] && ids[0] == 0 && ids[2] == 0);
    CHECK(names[3] && strcmp(names[3], "cuff") == 0 && ids[3] == 20);
    CHECK(names[4] && strcmp(names[4], "hem") == 0 && ids[4] == 10);
    CHECK(names[5] && strcmp(names[5], "tip") == 0 && ids[5] == 30);
    // Per entry: the pair copies the name, the list copies the pair; the pair and the read name are destroyed.
    CHECK(g_copies == 6 && g_destroys == 6);
    CHECK(g_groupCount == 2 && g_groups[0] == raw + 0xFC && g_groups[1] == raw + 0x108);
    long left; memcpy(&pos, g_stream + 4, 4); memcpy(&left, g_stream + 0x14, 4);
    CHECK(pos == full && left == 0 && g_overflows == 0);

    if (g_fail) { printf("PARTICLE_PROGRAM_READ_FAIL %d\n", g_fail); return 1; }
    printf("PARTICLE_PROGRAM_READ_OK\n");
    return 0;
}
