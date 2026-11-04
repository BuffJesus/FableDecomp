// Behaviour fixture for QuadricOptimiserInternals::COptimisedFace index queries (provider
// COptimisedFace_Queries_00ac6f50.cpp): remap of every matching corner, vertex / unique-vertex membership, first-corner
// search with -1 for a miss, order-independent identity over unique indices, and degeneracy on any repeated corner.
#include <stdio.h>
#include "fable_quadric_optimiser.h"

using QuadricOptimiserInternals::COptimisedFace;
static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static COptimisedFace Face(unsigned long a, unsigned long b, unsigned long c, unsigned long ua, unsigned long ub, unsigned long uc)
{
    COptimisedFace f;
    f.VertexIndices[0] = a; f.VertexIndices[1] = b; f.VertexIndices[2] = c;
    f.UniqueVertexIndices[0] = ua; f.UniqueVertexIndices[1] = ub; f.UniqueVertexIndices[2] = uc;
    f.Edges[0] = f.Edges[1] = f.Edges[2] = -7;
    return f;
}

int main()
{
    COptimisedFace f = Face(4, 9, 4, 10, 11, 12);
    f.RemapIndex(4, 6);
    CHECK(f.VertexIndices[0] == 6 && f.VertexIndices[1] == 9 && f.VertexIndices[2] == 6);
    CHECK(f.UniqueVertexIndices[0] == 10 && f.Edges[2] == -7);   // only corner indices change
    f.RemapIndex(5, 1);
    CHECK(f.VertexIndices[0] == 6 && f.VertexIndices[1] == 9 && f.VertexIndices[2] == 6);

    COptimisedFace g = Face(1, 2, 3, 20, 21, 22);
    CHECK(g.HasVertex(1) && g.HasVertex(2) && g.HasVertex(3) && !g.HasVertex(20) && !g.HasVertex(0));
    CHECK(g.HasUniqueVertex(20) && g.HasUniqueVertex(22) && !g.HasUniqueVertex(1));
    CHECK(g.FindVertex(1) == 0 && g.FindVertex(2) == 1 && g.FindVertex(3) == 2 && g.FindVertex(7) == -1);
    COptimisedFace twice = Face(5, 5, 8, 0, 0, 0);
    CHECK(twice.FindVertex(5) == 0);   // first corner wins

    COptimisedFace rotated = Face(7, 8, 9, 22, 20, 21);
    COptimisedFace other = Face(1, 2, 3, 20, 21, 23);
    CHECK(g.IsIdentical(rotated) && rotated.IsIdentical(g));
    CHECK(!g.IsIdentical(other) && !other.IsIdentical(g));
    CHECK(g.IsIdentical(g));

    CHECK(!g.IsDegenerate());
    CHECK(Face(1, 1, 2, 0, 0, 0).IsDegenerate());
    CHECK(Face(1, 2, 2, 0, 0, 0).IsDegenerate());
    CHECK(Face(2, 1, 2, 0, 0, 0).IsDegenerate());
    CHECK(f.IsDegenerate());

    if (g_fail) { printf("COPTIMISED_FACE_QUERIES_FAIL %d\n", g_fail); return 1; }
    printf("COPTIMISED_FACE_QUERIES_OK\n");
    return 0;
}
