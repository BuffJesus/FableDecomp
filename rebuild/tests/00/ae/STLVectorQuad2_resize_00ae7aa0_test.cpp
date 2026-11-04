// Behaviour fixture for _STL::vector<C3DQuad2>::resize(size_t) (provider STLVectorQuad2_resize_00ae7aa0.cpp; the fixture
// keeps the one-argument resize declaration-only so it calls the provider's instantiation). Checked: growing from empty
// and from a filled list gives exactly the requested size with the earlier quads kept; shrinking keeps the leading
// quads; resizing to the same size and to zero work.
#define FABLE_VECTOR_RESIZE_OUT_OF_LINE
#include <stdio.h>
#include <new>
#include "fable_particle_program.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

// STLport's out-of-line uninitialized_fill_n is another unit's (retail instantiation): supply a plain copy loop.
namespace _STL
{
template <> C3DQuad2* FABLE_FASTCALL uninitialized_fill_n<C3DQuad2>(C3DQuad2* first, size_t n, const C3DQuad2& x)
{
    for (size_t i = 0; i < n; ++i) new (first + i) C3DQuad2(x);
    return first + n;
}
}

int main()
{
    {
        _STL::vector<C3DQuad2> v;
        v.resize(3);
        CHECK(v.size() == 3);
        for (unsigned i = 0; i < 3; ++i)
            for (unsigned k = 0; k < 4; ++k)
                v[i].VertexIndex[k] = (unsigned short)(10 * i + k);
        v.resize(7);
        CHECK(v.size() == 7 && v[2].VertexIndex[3] == 23 && v[0].VertexIndex[0] == 0 && v[1].VertexIndex[2] == 12);
        v.resize(2);
        CHECK(v.size() == 2 && v[1].VertexIndex[1] == 11);
        v.resize(2);
        CHECK(v.size() == 2 && v[1].VertexIndex[3] == 13);
        v.resize(0);
        CHECK(v.empty());
    }
    if (g_fail) { printf("QUAD_RESIZE_FAIL %d\n", g_fail); return 1; }
    printf("QUAD_RESIZE_OK\n");
    return 0;
}
