// Behaviour fixture for _STL::vector<C3DClothRenderVertex>::resize(size_t, const C3DClothRenderVertex&) (provider
// STLVectorClothRenderVertex_resize_00ae7830.cpp). Checked: growing appends copies of the given vertex after the kept
// ones (from empty, within and beyond capacity); shrinking keeps the leading vertices and ignores the fill value.
#include <stdio.h>
#include <new>
#include "fable_particle_program.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static C3DClothRenderVertex V(unsigned short p, unsigned short t)
{
    C3DClothRenderVertex v;
    v.PositionIndex = p;
    v.TexCoordIndex = t;
    return v;
}

// STLport's out-of-line uninitialized_fill_n is another unit's (retail instantiation): supply a plain copy loop.
namespace _STL
{
template <> C3DClothRenderVertex* FABLE_FASTCALL uninitialized_fill_n<C3DClothRenderVertex>(C3DClothRenderVertex* first, size_t n, const C3DClothRenderVertex& x)
{
    for (size_t i = 0; i < n; ++i) new (first + i) C3DClothRenderVertex(x);
    return first + n;
}
}

int main()
{
    {
        _STL::vector<C3DClothRenderVertex> v;
        v.resize(2, V(1, 2));
        CHECK(v.size() == 2 && v[0].PositionIndex == 1 && v[1].TexCoordIndex == 2);
        v[1] = V(5, 6);
        v.resize(6, V(7, 8));
        CHECK(v.size() == 6 && v[1].PositionIndex == 5 && v[2].PositionIndex == 7 && v[5].TexCoordIndex == 8);
        v.resize(3, V(9, 9));
        CHECK(v.size() == 3 && v[1].TexCoordIndex == 6 && v[2].PositionIndex == 7);
        v.resize(4, V(3, 4));
        CHECK(v.size() == 4 && v[3].PositionIndex == 3 && v[2].TexCoordIndex == 8);
    }
    if (g_fail) { printf("RENDER_VERTEX_RESIZE_FAIL %d\n", g_fail); return 1; }
    printf("RENDER_VERTEX_RESIZE_OK\n");
    return 0;
}
