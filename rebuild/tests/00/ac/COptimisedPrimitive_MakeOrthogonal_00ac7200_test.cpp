// Behaviour fixture for QuadricOptimiserInternals::COptimisedPrimitive::MakeOrthogonal (provider
// COptimisedPrimitive_MakeOrthogonal_00ac7200.cpp), called on raw storage (the method reads nothing of the primitive).
// DELTA_NOTIONAL_ZERO (0.0001f) is defined here as retail defines it. Checked: the result is perpendicular to a unit
// axis and keeps the input's length; an input already perpendicular comes back unchanged; an input along the axis (no
// remainder) comes back as the (near) zero remainder; an input shorter than 1e-4 comes back unchanged; the inputs are
// not modified.
#include <stdio.h>
#include <math.h>
#include "fable_quadric_optimiser.h"

using namespace QuadricOptimiserInternals;
extern const float DELTA_NOTIONAL_ZERO = 0.0001f;
static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
static bool Near(double a, double b) { return fabs(a - b) <= 1e-5 * (fabs(b) > 1 ? fabs(b) : 1); }
static double Len(const C3DVector& v) { return sqrt((double)v.X * v.X + (double)v.Y * v.Y + (double)v.Z * v.Z); }

int main()
{
    static unsigned char raw[0xA0];
    const COptimisedPrimitive& p = *reinterpret_cast<const COptimisedPrimitive*>(raw);

    C3DVector axis(0, 0, 1);
    C3DVector v(3, 4, 12);
    C3DVector r = p.MakeOrthogonal(v, axis);
    CHECK(Near(r.Z, 0) && Near(Len(r), 13) && Near(r.X, 3 * 13 / 5.0) && Near(r.Y, 4 * 13 / 5.0));
    CHECK(v.X == 3 && v.Y == 4 && v.Z == 12 && axis.Z == 1);

    float s = (float)(1 / sqrt(3.0));
    C3DVector diag(s, s, s);
    C3DVector w(1, -2, 7);
    r = p.MakeOrthogonal(w, diag);
    CHECK(Near(r.X * s + r.Y * s + r.Z * s, 0) && Near(Len(r), Len(w)));

    C3DVector perp(5, -1, 0);
    r = p.MakeOrthogonal(perp, axis);
    CHECK(Near(r.X, 5) && Near(r.Y, -1) && r.Z == 0);

    C3DVector along(0, 0, 9);
    r = p.MakeOrthogonal(along, axis);
    CHECK(r.X == 0 && r.Y == 0 && r.Z == 0);

    C3DVector tiny(0.00002f, 0.00003f, 0.00004f);
    r = p.MakeOrthogonal(tiny, axis);
    CHECK(r.X == tiny.X && r.Y == tiny.Y && r.Z == tiny.Z);

    if (g_fail) { printf("MAKE_ORTHOGONAL_FAIL %d\n", g_fail); return 1; }
    printf("MAKE_ORTHOGONAL_OK\n");
    return 0;
}
