// Behaviour fixture for CParticleProgram::AddGroup (provider CParticleProgram_AddGroup_00ae37d0.cpp). The program is raw
// storage at retail offsets (ParticleIndices +0xDC, VertexIndices +0xE8, SimGroups +0xFC, NonSimGroups +0x108). The
// fixture supplies the group vector's push_back (appending into preallocated storage so back() is real), C3DGroup2's
// default constructor, SetBoneIndex and AddVertexBlend (recording). Checked: one new group per list, both on the source
// bone; each source blend reaches the simulated group re-indexed through ParticleIndices and/or the non-simulated group
// through VertexIndices, with its weight, skipping -1 entries and vertices beyond either map.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_particle_program.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

struct Added { const void* group; unsigned long vertex; float weight; };
static Added g_added[32];
static int g_addCount;

C3DGroup2::C3DGroup2() : BoneIndex(-1) {}
void C3DGroup2::SetBoneIndex(long index) { BoneIndex = index; }
void C3DGroup2::AddVertexBlend(const C3DVertexBlend2& blend)
{
    Added& a = g_added[g_addCount++ & 31]; a.group = this; a.vertex = blend.GetVertexIndex(); a.weight = blend.GetBlendValue();
}
namespace _STL
{
template <> void vector<C3DGroup2, allocator<C3DGroup2> >::push_back(const C3DGroup2& g)
{
    memcpy(this->_M_finish, &g, sizeof(C3DGroup2));   // the pushed group is the empty default one
    ++this->_M_finish;
}
}

static void SetVector(unsigned char* raw, unsigned offset, void* begin, unsigned count, unsigned elementSize)
{
    unsigned char* b = (unsigned char*)begin; unsigned char* e = b + count * elementSize;
    memcpy(raw + offset, &b, 4); memcpy(raw + offset + 4, &e, 4); memcpy(raw + offset + 8, &e, 4);
}

int main()
{
    static unsigned char raw[0x140];
    static long particles[4] = { 7, -1, 9, -1 };       // vertices 0..3
    static long vertices[3]  = { -1, 11, 12 };         // vertices 0..2
    static unsigned char simStore[4 * 16], nonSimStore[4 * 16];
    SetVector(raw, 0xDC, particles, 4, 4);
    SetVector(raw, 0xE8, vertices, 3, 4);
    SetVector(raw, 0xFC, simStore, 0, 16);
    SetVector(raw, 0x108, nonSimStore, 0, 16);
    CParticleProgram* program = reinterpret_cast<CParticleProgram*>(raw);

    // Source group: bone 5, blends for vertices 0, 1, 2, 3, 6.
    static unsigned long blends[10] = { 0, 0, 1, 0, 2, 0, 3, 0, 6, 0 };
    const float weights[5] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f };
    for (int i = 0; i < 5; ++i) memcpy(&blends[2 * i + 1], &weights[i], 4);
    unsigned long source[4] = { 5, 0, 0, 0 };
    unsigned long* bb = blends; unsigned long* be = blends + 10;
    memcpy(&source[1], &bb, 4); memcpy(&source[2], &be, 4); memcpy(&source[3], &be, 4);

    program->AddGroup(*reinterpret_cast<const C3DGroup2*>(source));

    unsigned char* simEnd; unsigned char* nonSimEnd;
    memcpy(&simEnd, raw + 0xFC + 4, 4); memcpy(&nonSimEnd, raw + 0x108 + 4, 4);
    CHECK(simEnd == simStore + 16 && nonSimEnd == nonSimStore + 16);
    long simBone, nonSimBone; memcpy(&simBone, simStore, 4); memcpy(&nonSimBone, nonSimStore, 4);
    CHECK(simBone == 5 && nonSimBone == 5);
    // Expected in source order: v0 -> sim 7; v1 -> nonsim 11; v2 -> sim 9 and nonsim 12; v3 -> none; v6 -> none.
    CHECK(g_addCount == 4);
    CHECK(g_added[0].group == simStore && g_added[0].vertex == 7 && g_added[0].weight == 0.1f);
    CHECK(g_added[1].group == nonSimStore && g_added[1].vertex == 11 && g_added[1].weight == 0.2f);
    CHECK(g_added[2].group == simStore && g_added[2].vertex == 9 && g_added[2].weight == 0.3f);
    CHECK(g_added[3].group == nonSimStore && g_added[3].vertex == 12 && g_added[3].weight == 0.3f);

    if (g_fail) { printf("PARTICLE_ADD_GROUP_FAIL %d\n", g_fail); return 1; }
    printf("PARTICLE_ADD_GROUP_OK\n");
    return 0;
}
