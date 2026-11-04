// Behaviour fixture for _STL::vector<C3DGroup2> (provider STLVectorGroup2_erase_00ae7f30.cpp). :erase(first, last): the groups after the range move down (blend lists assigned), the tail is destroyed, the size shrinks and the returned iterator is the first position; erasing an empty range changes nothing.
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
namespace _STL { template <> C3DGroup2* vector<C3DGroup2>::erase(C3DGroup2* first, C3DGroup2* last); }
// The blend-list assignment the moved groups use is another unit's instantiation: an element copy here.
namespace _STL
{
template <> vector<C3DVertexBlend2>& vector<C3DVertexBlend2>::operator=(const vector<C3DVertexBlend2>& o)
{
    if (this != &o) { _M_finish = _M_start; for (size_t i = 0; i < o.size(); ++i) push_back(o[i]); }
    return *this;
}
}
int main()
{
    {
        _STL::vector<C3DGroup2> v;
        for (long i = 0; i < 5; ++i) { C3DGroup2 g; g.SetBoneIndex(i); g.AddVertexBlend(C3DVertexBlend2(i, 0.5f)); v.push_back(g); }
        C3DGroup2* r = v.erase(v.begin() + 1, v.begin() + 3);
        CHECK(r == v.begin() + 1 && v.size() == 3);
        CHECK(v[0].GetBoneIndex() == 0 && v[1].GetBoneIndex() == 3 && v[2].GetBoneIndex() == 4);
        CHECK(v[1].GetVertexBlendCount() == 1 && v[1].PeekVertexBlend(0).VertexIndex == 3);
        r = v.erase(v.begin() + 2, v.begin() + 2);
        CHECK(r == v.begin() + 2 && v.size() == 3);
        v.erase(v.begin(), v.end());
        CHECK(v.empty());
    }
    if (g_fail) { printf("GROUP2_ERASE_FAIL %d\n", g_fail); return 1; }
    printf("GROUP2_ERASE_OK\n");
    return 0;
}
