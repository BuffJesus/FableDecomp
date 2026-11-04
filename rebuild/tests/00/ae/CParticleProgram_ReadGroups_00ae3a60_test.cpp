// Behaviour fixture for the private static CParticleProgram::ReadGroups (provider CParticleProgram_ReadGroups_00ae3a60.cpp).
// The stream is raw storage over an in-memory buffer (StreamPos +0x04, CurrentSourcePtr +0x0C, bytes left +0x14), so the
// provider's inline reads take the in-window path; the fixture supplies the group vector's resize and C3DGroup2's
// SetBoneIndex / Resize / GetVertexBlends over static storage. Checked: the list is sized to the stored count; each group
// gets its bone and exactly its blends (raw 8-byte records, including an empty group and a negative bone); the stream
// ends exactly after the data; the chunk-overflow reader is never needed.
#define FABLE_VECTOR_RESIZE_OUT_OF_LINE
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"
#include "fable_data_input_stream.h"

static int g_fail, g_overflows;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static unsigned long g_groupStore[8][4];          // C3DGroup2 = bone + blend vector
static unsigned char g_blendStore[8][64];
static size_t g_resized = (size_t)-1;
namespace _STL
{
template <> void vector<C3DGroup2, allocator<C3DGroup2> >::resize(size_t n)
{
    g_resized = n;
    memset(g_groupStore, 0, sizeof(g_groupStore));
    this->_M_start = reinterpret_cast<C3DGroup2*>(g_groupStore);
    this->_M_finish = this->_M_start + n;
    this->_M_end_of_storage._M_data = this->_M_finish;
}
}
void C3DGroup2::SetBoneIndex(long index) { BoneIndex = index; }
void C3DGroup2::Resize(unsigned long count)
{
    unsigned long i = (unsigned long)(reinterpret_cast<unsigned long*>(this) - &g_groupStore[0][0]) / 4;
    C3DVertexBlend2* b = reinterpret_cast<C3DVertexBlend2*>(g_blendStore[i]);
    VertexBlends._M_start = b; VertexBlends._M_finish = b + count; VertexBlends._M_end_of_storage._M_data = b + count;
}
C3DVertexBlend2* C3DGroup2::GetVertexBlends() { return VertexBlends._M_start; }
void CDataInputStream::ReadWithSrcChunkOverflow(void*, long) { ++g_overflows; }

int main()
{
    // count 3; {bone 5, 2 blends}, {bone -1, 0 blends}, {bone 7, 1 blend}
    unsigned long data[] = { 3,  5, 2, 10, 0x3F000000, 11, 0x3F000000,  0xFFFFFFFF, 0,  7, 1, 12, 0x3F800000 };
    static unsigned char stream[0x40];
    memset(stream, 0, sizeof(stream));
    unsigned char* src = (unsigned char*)data; long left = sizeof(data);
    memcpy(stream + 0x0C, &src, 4); memcpy(stream + 0x14, &left, 4);
    unsigned long list[3] = { 0, 0, 0 };

    extern void ReadGroupsEntry(void*, void*);
    ReadGroupsEntry(stream, list);

    CHECK(g_resized == 3 && g_overflows == 0);
    CHECK(g_groupStore[0][0] == 5 && g_groupStore[1][0] == 0xFFFFFFFFu && g_groupStore[2][0] == 7);
    CHECK(memcmp(g_blendStore[0], &data[3], 16) == 0 && memcmp(g_blendStore[2], &data[11], 8) == 0);
    const unsigned long expectedBlends[3] = { 2, 0, 1 };
    for (int i = 0; i < 3; ++i)   // each group's blend list is sized to exactly its stored count
        CHECK((g_groupStore[i][2] - g_groupStore[i][1]) / 8 == expectedBlends[i]);
    unsigned long pos; long rem; memcpy(&pos, stream + 0x04, 4); memcpy(&rem, stream + 0x14, 4);
    CHECK(pos == sizeof(data) && rem == 0);

    if (g_fail) { printf("PARTICLE_READ_GROUPS_FAIL %d\n", g_fail); return 1; }
    printf("PARTICLE_READ_GROUPS_OK\n");
    return 0;
}

// Private static: reached by its mangled name (fastcall: ecx = stream, edx = groups).
extern "C" void __fastcall CallReadGroups(void* stream, void* groups);
#pragma comment(linker, "/alternatename:@CallReadGroups@8=?ReadGroups@CParticleProgram@@CIXAAVCDataInputStream@@AAV?$CArray@VC3DGroup2@@@@@Z")
void ReadGroupsEntry(void* stream, void* groups) { CallReadGroups(stream, groups); }
