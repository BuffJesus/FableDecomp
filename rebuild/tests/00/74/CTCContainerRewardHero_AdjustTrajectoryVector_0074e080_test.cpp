// Behaviour fixture for CTCContainerRewardHero::AdjustTrajectoryVector (provider
// CTCContainerRewardHero_AdjustTrajectoryVector_0074e080.cpp), called through a derived probe on raw storage (the method
// reads nothing of the component). The fast reciprocal-root table is filled the way GFInitMaths fills it. Checked: X and
// Y become the vector's unit direction times 0.3; Z becomes its unit Z times 0.3 plus 0.2 * (itemIndex + 1); the scale
// is independent of the input length; negative and zero item indices follow the same formula.
#include <stdio.h>
#include <math.h>
#include "fable_tc_container_reward_hero.h"

unsigned long g_FableInvSqrtTable_013CE558[128];
static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

struct Probe : CTCContainerRewardHero
{
    void Adjust(long i, C3DVector& v) const { AdjustTrajectoryVector(i, v); }
};

static bool Near(double a, double b) { return fabs(a - b) <= 2e-4 * (fabs(b) > 1 ? fabs(b) : 1); }

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
    static unsigned char raw[0x20];
    const Probe& p = *reinterpret_cast<const Probe*>(raw);

    const float dirs[][3] = { { 3, 4, 0 }, { 0, 0, 10 }, { -1, 2, -2 }, { 0.01f, 0.02f, 0.02f }, { 500, -500, 250 } };
    const long items[] = { 0, 1, 4, -1, 7 };
    for (int k = 0; k < 5; ++k)
    {
        C3DVector v(dirs[k][0], dirs[k][1], dirs[k][2]);
        p.Adjust(items[k], v);
        double m = sqrt((double)dirs[k][0] * dirs[k][0] + (double)dirs[k][1] * dirs[k][1] + (double)dirs[k][2] * dirs[k][2]);
        CHECK(Near(v.X, 0.3 * dirs[k][0] / m) && Near(v.Y, 0.3 * dirs[k][1] / m));
        CHECK(Near(v.Z, 0.3 * dirs[k][2] / m + 0.2 * (items[k] + 1)));
    }

    if (g_fail) { printf("REWARD_TRAJECTORY_FAIL %d\n", g_fail); return 1; }
    printf("REWARD_TRAJECTORY_OK\n");
    return 0;
}
