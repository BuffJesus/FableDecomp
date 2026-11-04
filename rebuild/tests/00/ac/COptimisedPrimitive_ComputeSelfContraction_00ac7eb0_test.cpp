// Behaviour fixture for QuadricOptimiserInternals::COptimisedPrimitive::ComputeSelfContraction (provider
// COptimisedPrimitive_ComputeSelfContraction_00ac7eb0.cpp) on raw storage: the primitive's vertex array (+0x10) points at
// retail-stride (0xB8) vertex records (Position +0x00, Normal +0x0C, TextureCoord +0x18) and its optimisation parameters
// sit at +0x5C; the contraction's first sub-contraction names the pair. Checked: Cost = |dT|^2 * UnsplitTextureWeight +
// |dN|^2 * UnsplitNormalWeight, plus HighPenalty only when the normals point apart (negative dot); a zero dot gets no
// penalty; ResultingPosition is the first vertex's position; Weight2 is 0.5; the sub-contraction list and flags are
// untouched; the pair's order matters only through the position taken.
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <new>
#include "fable_quadric_optimiser.h"
#include "fable_stlport_vector_impl.h"

using namespace QuadricOptimiserInternals;
static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
static bool Near(double a, double b) { return fabs(a - b) <= 1e-4 * (fabs(b) > 1 ? fabs(b) : 1); }

static unsigned char g_vertices[4][0xB8];
static unsigned char g_prim[0x84];
static void Set(int i, const float* pos, const float* normal, const float* tex)
{
    memcpy(g_vertices[i], pos, 12); memcpy(g_vertices[i] + 0xC, normal, 12); memcpy(g_vertices[i] + 0x18, tex, 12);
}

int main()
{
    memset(g_vertices, 0, sizeof(g_vertices));
    void* begin = g_vertices; void* end = g_vertices + 4;
    memcpy(g_prim + 0x10, &begin, 4); memcpy(g_prim + 0x14, &end, 4); memcpy(g_prim + 0x18, &end, 4);
    COptimisedPrimitive* prim = reinterpret_cast<COptimisedPrimitive*>(g_prim);
    prim->Parameters.UnsplitTextureWeight = 3.0f;
    prim->Parameters.UnsplitNormalWeight = 5.0f;
    prim->Parameters.HighPenalty = 1000.0f;

    const float p0[3] = { 1, 2, 3 }, p1[3] = { 1, 2, 3 }, p2[3] = { 7, 8, 9 };
    const float up[3] = { 0, 0, 1 }, side[3] = { 1, 0, 0 }, down[3] = { 0, 0, -1 }, tilt[3] = { 0.6f, 0, 0.8f };
    const float t0[3] = { 0, 0, 0 }, t1[3] = { 3, 4, 0 }, t2[3] = { 0.5f, 0, 0 };
    Set(0, p0, up, t0); Set(1, p1, tilt, t1); Set(2, p2, down, t2); Set(3, p0, side, t0);

    CVertexPairContraction c;
    c.Exists = true; c.Unsplit = true;
    CSubContraction s; s.Index1 = 0; s.Index2 = 1; c.SubContractions.push_back(s);
    prim->ComputeSelfContraction(c);
    double dn2 = 0.6 * 0.6 + 0.2 * 0.2;                                   // up - tilt
    CHECK(Near(c.Cost, 25.0 * 3 + dn2 * 5));
    CHECK(c.ResultingPosition.X == 1 && c.ResultingPosition.Y == 2 && c.ResultingPosition.Z == 3 && c.Weight2 == 0.5f);
    CHECK(c.SubContractions.size() == 1 && c.SubContractions[0].Index2 == 1 && c.Exists && c.Unsplit);

    c.SubContractions[0].Index1 = 2; c.SubContractions[0].Index2 = 0;      // opposite normals: penalty
    prim->ComputeSelfContraction(c);
    CHECK(Near(c.Cost, 0.25 * 3 + 4.0 * 5 + 1000));
    CHECK(c.ResultingPosition.X == 7 && c.ResultingPosition.Z == 9);

    c.SubContractions[0].Index1 = 0; c.SubContractions[0].Index2 = 3;      // perpendicular normals: no penalty
    prim->ComputeSelfContraction(c);
    CHECK(Near(c.Cost, 2.0 * 5));

    if (g_fail) { printf("SELF_CONTRACTION_FAIL %d\n", g_fail); return 1; }
    printf("SELF_CONTRACTION_OK\n");
    return 0;
}
