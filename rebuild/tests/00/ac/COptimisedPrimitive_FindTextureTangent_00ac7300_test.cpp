// Behaviour fixture for QuadricOptimiserInternals::COptimisedPrimitive::FindTextureTangent (provider
// COptimisedPrimitive_FindTextureTangent_00ac7300.cpp), called on raw storage (it reads nothing of the primitive). The
// fast reciprocal-root table and DELTA_NOTIONAL_ZERO (0.0001f) are defined here as GFInitMaths / retail define them.
// Checked: for a coordinate that is a linear function of position in the triangle's plane, the result is that
// function's gradient within the plane (direction and magnitude), whatever the vertex order and winding; a coordinate
// range within 1e-4 gives the zero vector; the inputs are not modified.
#include <stdio.h>
#include <math.h>
#include "fable_quadric_optimiser.h"

using namespace QuadricOptimiserInternals;
unsigned long g_FableInvSqrtTable_013CE558[128];
extern const float DELTA_NOTIONAL_ZERO = 0.0001f;
static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
static bool Near(double a, double b) { return fabs(a - b) <= 2e-3 * (fabs(b) > 1 ? fabs(b) : 1); }

static COptimisedPrimitive* g_p;
static C3DVector Tangent(const C3DVector& a, const C3DVector& b, const C3DVector& c, const C3DVector& gradient, float offset)
{
    float ta = a.X * gradient.X + a.Y * gradient.Y + a.Z * gradient.Z + offset;
    float tb = b.X * gradient.X + b.Y * gradient.Y + b.Z * gradient.Z + offset;
    float tc = c.X * gradient.X + c.Y * gradient.Y + c.Z * gradient.Z + offset;
    return g_p->FindTextureTangent(a, b, c, ta, tb, tc);
}
static bool Is(const C3DVector& v, double x, double y, double z) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z); }

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
    static unsigned char raw[0xA0];
    g_p = reinterpret_cast<COptimisedPrimitive*>(raw);

    // Triangle in the z = 0 plane.
    C3DVector a(0, 0, 0), b(2, 0, 0), c(0, 3, 0);
    CHECK(Is(Tangent(a, b, c, C3DVector(1, 0, 0), 0), 1, 0, 0));        // t = x
    CHECK(Is(Tangent(a, b, c, C3DVector(0, 2, 0), 5), 0, 2, 0));        // t = 2y + 5
    CHECK(Is(Tangent(c, b, a, C3DVector(0, 2, 0), 5), 0, 2, 0));        // other winding
    CHECK(Is(Tangent(b, c, a, C3DVector(-0.5f, 0.25f, 0), 1), -0.5, 0.25, 0));
    // A gradient with an out-of-plane part only keeps its in-plane projection.
    CHECK(Is(Tangent(a, b, c, C3DVector(1, 1, 7), 0), 1, 1, 0));
    // Tilted triangle: plane x + y + z = 3; gradient (1, -1, 0) lies in it.
    C3DVector d(3, 0, 0), e(0, 3, 0), f(0, 0, 3);
    CHECK(Is(Tangent(d, e, f, C3DVector(1, -1, 0), 0), 1, -1, 0));
    // No coordinate range: zero vector.
    C3DVector z = g_p->FindTextureTangent(a, b, c, 4.0f, 4.00005f, 4.0f);
    CHECK(z.X == 0 && z.Y == 0 && z.Z == 0);
    CHECK(a.X == 0 && b.X == 2 && c.Y == 3);

    if (g_fail) { printf("TEXTURE_TANGENT_FAIL %d\n", g_fail); return 1; }
    printf("TEXTURE_TANGENT_OK\n");
    return 0;
}
