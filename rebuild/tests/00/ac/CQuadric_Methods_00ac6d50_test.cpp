// Behaviour fixture for QuadricOptimiserInternals::CQuadric EvaluateError / operator+= / operator*= / Initialise
// (provider CQuadric_Methods_00ac6d50.cpp). Checked: Initialise builds the outer product of (n, d) with d = -n.p and
// keeps the weight; EvaluateError of that quadric at a point is (n.v + d)^2 (zero on the plane); a general quadric
// evaluates to the full expansion; += adds all eleven fields and *= scales all eleven (the weight included).
#include <stdio.h>
#include <math.h>
#include "fable_quadric_optimiser.h"

using QuadricOptimiserInternals::CQuadric;
static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
static bool Near(double a, double b) { return fabs(a - b) <= 1e-5 * (fabs(b) > 1 ? fabs(b) : 1); }

int main()
{
    CQuadric q;
    C3DVector n(0.5f, 0.6f, 0.8f), p(1.0f, 2.0f, 3.0f);
    q.Initialise(n, p, 2.5f);
    double d = -(0.5 * 1 + 0.6 * 2 + 0.8 * 3);
    CHECK(Near(q.AA, 0.25) && Near(q.AB, 0.3) && Near(q.AC, 0.4) && Near(q.AD, 0.5 * d));
    CHECK(Near(q.BB, 0.36) && Near(q.BC, 0.48) && Near(q.BD, 0.6 * d) && Near(q.CC, 0.64) && Near(q.CD, 0.8 * d));
    CHECK(Near(q.DD, d * d) && q.Weight == 2.5);
    CHECK(Near(q.EvaluateError(p), 0.0));
    C3DVector v(4.0f, 5.0f, -1.0f);
    double e = 0.5 * 4 + 0.6 * 5 + 0.8 * -1 + d;
    CHECK(Near(q.EvaluateError(v), e * e));

    CQuadric g;
    g.AA = 1; g.AB = 2; g.AC = 3; g.AD = 4; g.BB = 5; g.BC = 6; g.BD = 7; g.CC = 8; g.CD = 9; g.DD = 10; g.Weight = 11;
    double x = 2, y = -1, z = 0.5;
    double want = 1 * x * x + 2 * 2 * x * y + 2 * 3 * x * z + 2 * 4 * x + 5 * y * y + 2 * 6 * y * z + 2 * 7 * y + 8 * z * z + 2 * 9 * z + 10;
    CHECK(Near(g.EvaluateError(C3DVector(2.0f, -1.0f, 0.5f)), want));

    CQuadric h = g;
    h += g;
    const double* hp = &h.AA; const double* gp = &g.AA;
    for (int i = 0; i < 11; ++i) CHECK(hp[i] == 2 * gp[i]);
    h *= 0.25;
    for (int i = 0; i < 11; ++i) CHECK(hp[i] == 0.5 * gp[i]);

    if (g_fail) { printf("QUADRIC_FAIL %d\n", g_fail); return 1; }
    printf("QUADRIC_OK\n");
    return 0;
}
