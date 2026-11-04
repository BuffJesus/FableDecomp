// Behaviour fixture for _STL::vector<C2DVector>::resize(size_t) (provider STLVector2DVector_resize_00ae7b10.cpp; the fixture
// keeps the one-argument resize declaration-only so it calls the provider's instantiation). Checked: growing from empty
// and from a filled list gives exactly the requested size with the earlier coordinates kept; shrinking keeps the leading
// coordinates; resizing to the same size and to zero work.
#define FABLE_VECTOR_RESIZE_OUT_OF_LINE
#include <stdio.h>
#include <new>
#include "fable_particle_program.h"
#include "fable_2d_vector.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

// STLport's out-of-line uninitialized_fill_n is another unit's (retail instantiation): supply a plain copy loop.
namespace _STL
{
template <> C2DVector* FABLE_FASTCALL uninitialized_fill_n<C2DVector>(C2DVector* first, size_t n, const C2DVector& x)
{
    for (size_t i = 0; i < n; ++i) new (first + i) C2DVector(x);
    return first + n;
}
}

int main()
{
    {
        _STL::vector<C2DVector> v;
        v.resize(3);
        CHECK(v.size() == 3);
        for (unsigned i = 0; i < 3; ++i) { v[i].X = (float)(10 * i); v[i].Y = (float)(10 * i + 1); }
        v.resize(7);
        CHECK(v.size() == 7 && v[2].X == 20 && v[2].Y == 21 && v[0].Y == 1);
        v.resize(2);
        CHECK(v.size() == 2 && v[1].Y == 11);
        v.resize(2);
        CHECK(v.size() == 2 && v[1].X == 10);
        v.resize(0);
        CHECK(v.empty());
    }
    if (g_fail) { printf("UV_RESIZE_FAIL %d\n", g_fail); return 1; }
    printf("UV_RESIZE_OK\n");
    return 0;
}
