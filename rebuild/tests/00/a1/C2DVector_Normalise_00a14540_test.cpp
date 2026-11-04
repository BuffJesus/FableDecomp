// Behaviour fixture for C2DVector::Normalise (provider C2DVector_Normalise_00a14540.cpp). The fast reciprocal-root table
// is defined here and filled the way GFInitMaths (0x00A0DB60) fills it. Checked: the result has unit length to within
// the one-Newton-step accuracy (worst case about 1e-4 relative; 2e-4 allowed), keeps the direction (components scale by the same
// positive factor, signs kept), works across magnitudes from 1e-3 to 1e6, and returns the vector itself.
#include <stdio.h>
#include <math.h>
#include "fable_2d_vector.h"

unsigned long g_FableInvSqrtTable_013CE558[128];
static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

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

    const float cases[][2] = { { 3, 4 }, { -3, 4 }, { 0.001f, -0.002f }, { 1e6f, 1 }, { 0, -7 }, { 1, 1 }, { 123.25f, -0.5f } };
    for (int k = 0; k < (int)(sizeof(cases) / sizeof(cases[0])); ++k)
    {
        C2DVector v(cases[k][0], cases[k][1]);
        C2DVector& r = v.Normalise();
        CHECK(&r == &v);
        double len = sqrt((double)v.X * v.X + (double)v.Y * v.Y);
        CHECK(fabs(len - 1.0) < 2e-4);
        double m = sqrt((double)cases[k][0] * cases[k][0] + (double)cases[k][1] * cases[k][1]);
        CHECK(fabs(v.X - cases[k][0] / m) < 3e-4 && fabs(v.Y - cases[k][1] / m) < 3e-4);
        CHECK((v.X < 0) == (cases[k][0] < 0) && (v.Y < 0) == (cases[k][1] < 0));
    }
    C2DVector a(3, 4);
    a.Normalise();
    CHECK(fabs(a.X - 0.6f) < 2e-4 && fabs(a.Y - 0.8f) < 2e-4);

    if (g_fail) { printf("VECTOR2_NORMALISE_FAIL %d\n", g_fail); return 1; }
    printf("VECTOR2_NORMALISE_OK\n");
    return 0;
}
