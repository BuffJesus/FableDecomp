// Behaviour fixture for QuadricOptimiserInternals::COptimisedPrimitive::GetFaceNormal (provider
// COptimisedPrimitive_GetFaceNormal_00ac7cb0.cpp), on raw storage: the primitive's vertex array (+0x10) points at
// retail-stride (0xB8) vertex records whose Position is at +0. The fast reciprocal-root table and DELTA_NOTIONAL_ZERO
// (0.0001f) are defined here as GFInitMaths / retail define them. Checked: the result is the unit normal (p2 - p0) x
// (p1 - p0), i.e. the engine's winding; it depends only on the three indexed vertices; a degenerate face whose cross
// product has squared length <= 1e-8 comes back unnormalised (here zero or tiny); a face with a small but non-degenerate
// normal is still normalised; the face itself is not modified.
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fable_quadric_optimiser.h"

using namespace QuadricOptimiserInternals;
unsigned long g_FableInvSqrtTable_013CE558[128];
extern const float DELTA_NOTIONAL_ZERO = 0.0001f;
static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static unsigned char g_vertices[8][0xB8];
static unsigned char g_prim[0x20];
static void SetPos(int i, float x, float y, float z) { float p[3] = { x, y, z }; memcpy(g_vertices[i], p, 12); }
static C3DVector Normal(unsigned long a, unsigned long b, unsigned long c)
{
    COptimisedFace f; memset(&f, 0, sizeof(f)); f.VertexIndices[0] = a; f.VertexIndices[1] = b; f.VertexIndices[2] = c;
    COptimisedFace keep = f;
    C3DVector n = reinterpret_cast<const COptimisedPrimitive*>(g_prim)->GetFaceNormal(f);
    CHECK(!memcmp(&keep, &f, sizeof(f)));
    return n;
}
static bool Near(float a, float b, float tol) { return fabs(a - b) <= tol; }

int main()
{
    union { unsigned long bits; float value; } input, reciprocal;
    for (unsigned long i = 0; i < 128; ++i)
    {
        input.bits = (i | 0x1f80UL) << 17;
        reciprocal.value = (float)(1.0 / sqrt(input.value));
        g_FableInvSqrtTable_013CE558[i] = ((reciprocal.bits + 0x2000UL) >> 15) & 0xffUL;
    }
    g_FableInvSqrtTable_013CE558[64] = 255;
    memset(g_vertices, 0x7F, sizeof(g_vertices));
    void* begin = g_vertices; void* end = g_vertices + 8;
    memcpy(g_prim + 0x10, &begin, 4); memcpy(g_prim + 0x14, &end, 4); memcpy(g_prim + 0x18, &end, 4);

    SetPos(0, 0, 0, 0); SetPos(1, 2, 0, 0); SetPos(2, 0, 3, 0);
    C3DVector n = Normal(0, 1, 2);                       // (p2-p0) x (p1-p0) = (0,3,0) x (2,0,0) = (0,0,-6) -> (0,0,-1)
    CHECK(Near(n.X, 0, 1e-6f) && Near(n.Y, 0, 1e-6f) && Near(n.Z, -1, 2e-4f));
    n = Normal(0, 2, 1);
    CHECK(Near(n.Z, 1, 2e-4f) && Near(n.X, 0, 1e-6f));

    SetPos(3, 1, 1, 1); SetPos(4, 1, 5, 1); SetPos(5, 1, 1, 9);   // (p2-p0) x (p1-p0) = (0,0,8) x (0,4,0) = (-32,0,0)
    n = Normal(3, 4, 5);
    CHECK(Near(n.X, -1, 2e-4f) && Near(n.Y, 0, 1e-6f) && Near(n.Z, 0, 1e-6f));

    SetPos(6, 1, 1, 1);                                            // collinear: zero normal, left as is
    n = Normal(3, 6, 6);
    CHECK(n.X == 0 && n.Y == 0 && n.Z == 0);
    SetPos(7, 1, 1.0000012f, 1);                                   // one ulp: |cross|^2 ~ 9e-11 < 1e-8, unnormalised
    n = Normal(3, 7, 5);
    CHECK(fabs(n.X) < 1e-3f && n.X != 0);
    SetPos(7, 1, 1.01f, 1);                                        // cross (-0.08,0,0): squared 6.4e-3 > 1e-8, normalised
    n = Normal(3, 7, 5);
    CHECK(Near(n.X, -1, 2e-4f));

    if (g_fail) { printf("FACE_NORMAL_FAIL %d\n", g_fail); return 1; }
    printf("FACE_NORMAL_OK\n");
    return 0;
}
