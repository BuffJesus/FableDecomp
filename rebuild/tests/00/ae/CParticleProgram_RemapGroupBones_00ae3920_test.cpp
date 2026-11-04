// Behaviour fixture for the private static CParticleProgram::RemapBoneIndices(map&, CArray<C3DGroup2>&) (provider
// CParticleProgram_RemapGroupBones_00ae3920.cpp). The bone map is a small STLport red-black tree built by hand at the
// node offsets the provider's inline search walks (parent/left/right +4/+8/+0xC, key/value +0x10/+0x14); the fixture
// supplies map::operator[] over it, the group vector's reserve/push_back/operator= (recording, not storing) and
// C3DGroup2::SetBoneIndex. Checked: the new list is reserved to the old size; exactly the groups whose bone is in the
// map are moved to the mapped bone (in order) and kept; groups with unmapped bones are dropped untouched; and the list
// is finally assigned from the rebuilt one.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

// ---- the hand-built tree --------------------------------------------------------------------------------------------
struct Node { int color; Node* parent; Node* left; Node* right; unsigned long key; unsigned long value; };
static Node g_header, g_nodes[3];
static unsigned char g_map[16];   // map storage: _M_header._M_data (+0), node count, comparator

static void BuildMap()
{
    // keys 10 (->100), 20 (->200), 30 (->300): root 20, left 10, right 30
    memset(&g_header, 0, sizeof(g_header)); memset(g_nodes, 0, sizeof(g_nodes));
    Node* n10 = &g_nodes[0]; Node* n20 = &g_nodes[1]; Node* n30 = &g_nodes[2];
    n10->key = 10; n10->value = 100; n20->key = 20; n20->value = 200; n30->key = 30; n30->value = 300;
    n20->left = n10; n20->right = n30; n10->parent = n20; n30->parent = n20; n20->parent = &g_header;
    g_header.parent = n20; g_header.left = n10; g_header.right = n30;
    Node* header = &g_header; memcpy(g_map, &header, 4);
    unsigned long count = 3; memcpy(g_map + 4, &count, 4);
}

static int g_lookups;
namespace _STL
{
template <> unsigned long& map<unsigned long, unsigned long, less<unsigned long>, allocator<pair<const unsigned long, unsigned long> > >::operator[](const unsigned long& key)
{
    ++g_lookups;
    for (int i = 0; i < 3; ++i) if (g_nodes[i].key == key) return g_nodes[i].value;
    static unsigned long missing; CHECK(!"operator[] on a missing key"); return missing;
}
static size_t g_reserved = (size_t)-1;
static int g_pushes, g_assigns;
static long g_pushedBone[8];
static const void* g_assignedTo;
template <> void vector<C3DGroup2, allocator<C3DGroup2> >::reserve(size_t n) { g_reserved = n; }
template <> void vector<C3DGroup2, allocator<C3DGroup2> >::push_back(const C3DGroup2& g) { g_pushedBone[g_pushes++ & 7] = g.GetBoneIndex(); }
template <> vector<C3DGroup2, allocator<C3DGroup2> >& vector<C3DGroup2, allocator<C3DGroup2> >::operator=(const vector<C3DGroup2, allocator<C3DGroup2> >&)
{
    ++g_assigns; g_assignedTo = this; return *this;
}
}
void C3DGroup2::SetBoneIndex(long index) { BoneIndex = index; }

// Private static: reached by its mangled name (fastcall: ecx = map, edx = groups).
extern "C" void __fastcall CallRemap(void* map, void* groups);
#pragma comment(linker, "/alternatename:@CallRemap@8=?RemapBoneIndices@CParticleProgram@@CIXAAV?$map@KKU?$less@K@_STL@@V?$allocator@U?$pair@$$CBKK@_STL@@@2@@_STL@@AAV?$CArray@VC3DGroup2@@@@@Z")

int main()
{
    BuildMap();
    // Five groups (16 bytes each: BoneIndex + empty CArray): bones 20, 5, 10, 31, 30.
    static unsigned long groups[5][4];
    const long bones[5] = { 20, 5, 10, 31, 30 };
    memset(groups, 0, sizeof(groups));
    for (int i = 0; i < 5; ++i) groups[i][0] = (unsigned long)bones[i];
    unsigned long* begin = &groups[0][0]; unsigned long* end = &groups[5][0];
    unsigned long list[3]; memcpy(&list[0], &begin, 4); memcpy(&list[1], &end, 4); memcpy(&list[2], &end, 4);

    CallRemap(g_map, list);

    CHECK(_STL::g_reserved == 5);
    CHECK(_STL::g_pushes == 3 && _STL::g_pushedBone[0] == 200 && _STL::g_pushedBone[1] == 100 && _STL::g_pushedBone[2] == 300);
    CHECK(groups[0][0] == 200 && groups[1][0] == 5 && groups[2][0] == 100 && groups[3][0] == 31 && groups[4][0] == 300);
    CHECK(g_lookups == 3);
    CHECK(_STL::g_assigns == 1 && _STL::g_assignedTo == list);

    if (g_fail) { printf("PARTICLE_REMAP_GROUP_BONES_FAIL %d\n", g_fail); return 1; }
    printf("PARTICLE_REMAP_GROUP_BONES_OK\n");
    return 0;
}
