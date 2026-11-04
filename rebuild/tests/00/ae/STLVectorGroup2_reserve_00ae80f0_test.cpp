// Behaviour fixture for _STL::vector<C3DGroup2> (provider STLVectorGroup2_reserve_00ae80f0.cpp). :reserve(n): a larger request reallocates to exactly n, copying every group (bone and blends) and keeping the size; a request within the capacity changes nothing.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_particle_program.h"
#include "fable_stlport_vector_impl.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
C3DGroup2::C3DGroup2() : BoneIndex(-1) {}
void C3DGroup2::SetBoneIndex(long index) { BoneIndex = index; }
void C3DGroup2::AddVertexBlend(const C3DVertexBlend2& blend) { VertexBlends.push_back(blend); }
namespace _STL { template <> void vector<C3DGroup2>::reserve(size_t n); }
int main()
{
    {
        _STL::vector<C3DGroup2> v;
        v.reserve(2);
        CHECK(v.capacity() == 2 && v.empty());
        C3DGroup2 g; g.SetBoneIndex(7); g.AddVertexBlend(C3DVertexBlend2(9, 1.0f)); v.push_back(g);
        const C3DGroup2* old = v.begin();
        v.reserve(8);
        CHECK(v.capacity() == 8 && v.size() == 1 && v.begin() != old && v[0].GetBoneIndex() == 7 && v[0].PeekVertexBlend(0).VertexIndex == 9);
        const C3DGroup2* now = v.begin();
        v.reserve(3);
        CHECK(v.capacity() == 8 && v.begin() == now);
    }
    if (g_fail) { printf("GROUP2_RESERVE_FAIL %d\n", g_fail); return 1; }
    printf("GROUP2_RESERVE_OK\n");
    return 0;
}
