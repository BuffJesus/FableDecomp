// Behaviour fixture for _STL::vector<C3DGroup2>::resize(size_t) (provider STLVectorGroup2_resize_00ae8540.cpp). C3DGroup2's
// default constructor (bone -1, no blends) and SetBoneIndex are supplied here. Checked: growing appends default groups
// after the kept ones, which keep their bones; shrinking keeps the leading groups.
#define FABLE_VECTOR_RESIZE_OUT_OF_LINE
#include <stdio.h>
#include <new>
#include "fable_particle_program.h"

static int g_fail, g_defaults;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

C3DGroup2::C3DGroup2() : BoneIndex(-1) { ++g_defaults; }
void C3DGroup2::SetBoneIndex(long index) { BoneIndex = index; }

// STLport's out-of-line uninitialized_fill_n is another unit's (retail instantiation): supply a plain copy loop.
namespace _STL
{
template <> C3DGroup2* FABLE_FASTCALL uninitialized_fill_n<C3DGroup2>(C3DGroup2* first, size_t n, const C3DGroup2& x)
{
    for (size_t i = 0; i < n; ++i) new (first + i) C3DGroup2(x);
    return first + n;
}
}
// Not reached when resizing at the end (no group is moved): flag it if it is.
template <> _STL::vector<C3DVertexBlend2>& _STL::vector<C3DVertexBlend2>::operator=(const _STL::vector<C3DVertexBlend2>&)
{
    CHECK(!"no group is reassigned"); return *this;
}

int main()
{
    {
        _STL::vector<C3DGroup2> v;
        v.resize(2);
        CHECK(v.size() == 2 && v[0].GetBoneIndex() == -1 && v[1].GetBoneIndex() == -1 && g_defaults >= 1);
        v[0].SetBoneIndex(4);
        v[1].SetBoneIndex(5);
        v.resize(6);
        CHECK(v.size() == 6 && v[0].GetBoneIndex() == 4 && v[1].GetBoneIndex() == 5 && v[5].GetBoneIndex() == -1);
        v.resize(1);
        CHECK(v.size() == 1 && v[0].GetBoneIndex() == 4);
    }
    if (g_fail) { printf("GROUP2_VECTOR_RESIZE_FAIL %d\n", g_fail); return 1; }
    printf("GROUP2_VECTOR_RESIZE_OK\n");
    return 0;
}
