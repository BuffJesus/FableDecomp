// Behaviour fixture for QuadricOptimiserInternals::CVertexPairContraction RemapIndex / HasVertex / CleanUp (provider
// CVertexPairContraction_Methods_00ac7940.cpp). Checked: RemapIndex renames the old index on either side of every pair
// and leaves other indices alone; HasVertex finds an index on either side and is false for an empty list or an unused
// index; CleanUp removes every later repeat of a pair (ordered: (a,b) and (b,a) are distinct), keeps the first of each
// in order, and leaves the flags alone.
#include <stdio.h>
#include <new>
#include "fable_quadric_optimiser.h"
#include "fable_stlport_vector_impl.h"

using namespace QuadricOptimiserInternals;
static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static CSubContraction P(unsigned long a, unsigned long b) { CSubContraction p; p.Index1 = a; p.Index2 = b; return p; }
static void Set(CVertexPairContraction& c, const unsigned long* v, int n)
{
    c.SubContractions.clear();
    for (int i = 0; i < n; ++i) c.SubContractions.push_back(P(v[2 * i], v[2 * i + 1]));
}
static bool Is(const CVertexPairContraction& c, const unsigned long* v, int n)
{
    if ((int)c.SubContractions.size() != n) return false;
    for (int i = 0; i < n; ++i)
        if (c.SubContractions[i].Index1 != v[2 * i] || c.SubContractions[i].Index2 != v[2 * i + 1]) return false;
    return true;
}

int main()
{
    CVertexPairContraction c;
    c.Exists = true; c.Unsplit = false;
    CHECK(!c.HasVertex(0));

    const unsigned long a[] = { 1, 2, 2, 3, 4, 2, 5, 6 };
    Set(c, a, 4);
    c.RemapIndex(2, 9);
    const unsigned long ar[] = { 1, 9, 9, 3, 4, 9, 5, 6 };
    CHECK(Is(c, ar, 4));
    c.RemapIndex(7, 8);
    CHECK(Is(c, ar, 4));
    c.RemapIndex(5, 5);
    CHECK(Is(c, ar, 4));
    CHECK(c.HasVertex(1) && c.HasVertex(9) && c.HasVertex(6) && c.HasVertex(4) && !c.HasVertex(2) && !c.HasVertex(7));

    const unsigned long d[] = { 1, 2, 3, 4, 1, 2, 2, 1, 1, 2, 3, 4, 3, 5, 5, 6, 5, 6, 5, 6 };
    Set(c, d, 10);
    c.CleanUp();
    const unsigned long dr[] = { 1, 2, 3, 4, 2, 1, 3, 5, 5, 6 };
    CHECK(Is(c, dr, 5));
    c.CleanUp();
    CHECK(Is(c, dr, 5));
    const unsigned long s[] = { 7, 7, 7, 7 };
    Set(c, s, 2);
    c.CleanUp();
    CHECK(Is(c, s, 1));
    Set(c, s, 0);
    c.CleanUp();
    CHECK(c.SubContractions.size() == 0 && c.Exists && !c.Unsplit);

    if (g_fail) { printf("VERTEX_PAIR_CONTRACTION_FAIL %d\n", g_fail); return 1; }
    printf("VERTEX_PAIR_CONTRACTION_OK\n");
    return 0;
}
