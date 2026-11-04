// Behaviour fixture for C3DGroup2::Resize (provider C3DGroup2_Resize_00ac41d0.cpp) on raw storage (bone + blend vector):
// growing gives exactly the requested count; shrinking keeps the leading blends unchanged; the bone is untouched.
#include <stdio.h>
#include <string.h>
#include "fable_chunked_file.h"
#include "fable_3d_group2.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

// The out-of-line STLport fill is another unit's (retail instantiation); supply a plain loop.
namespace _STL
{
template <> C3DVertexBlend2* FABLE_FASTCALL uninitialized_fill_n<C3DVertexBlend2>(C3DVertexBlend2* first, size_t n, const C3DVertexBlend2& x)
{
    for (size_t i = 0; i < n; ++i) first[i] = x;
    return first + n;
}
}

static unsigned long Count(const unsigned long* g) { return (g[2] - g[1]) / 8; }

int main()
{
    unsigned long g[4] = { 6, 0, 0, 0 };
    C3DGroup2* group = reinterpret_cast<C3DGroup2*>(g);
    group->Resize(5);
    CHECK(Count(g) == 5 && g[0] == 6);
    C3DVertexBlend2* b = reinterpret_cast<C3DVertexBlend2*>(g[1]);
    for (unsigned long i = 0; i < 5; ++i) { b[i].VertexIndex = 40 + i; b[i].BlendValue = 0.5f; }
    group->Resize(2);
    b = reinterpret_cast<C3DVertexBlend2*>(g[1]);
    CHECK(Count(g) == 2 && b[0].VertexIndex == 40 && b[1].VertexIndex == 41 && b[1].BlendValue == 0.5f);
    group->Resize(8);
    b = reinterpret_cast<C3DVertexBlend2*>(g[1]);
    CHECK(Count(g) == 8 && b[0].VertexIndex == 40 && b[1].VertexIndex == 41 && g[0] == 6);
    if (g_fail) { printf("GROUP2_RESIZE_FAIL %d\n", g_fail); return 1; }
    printf("GROUP2_RESIZE_OK\n");
    return 0;
}
