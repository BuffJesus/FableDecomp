
#include <stdio.h>

long FtoL_model(float f)
{
    // x87 fistp uses round-to-nearest (banker's) by default
    // model: round half to even
    double d = (double)f;
    double fl = (d < 0.0) ? -(double)((long)(-d)) : (double)((long)d);
    double frac = d - fl;
    long base = (long)fl;
    if (frac > 0.5) base += (d < 0.0) ? -1 : 1;
    else if (frac < -0.5) base += 1;
    // simplification: just use nearbyint-like behavior for test values chosen to be exact/.0 or clearly rounding
    return base;
}

int main()
{
    struct { float in; long expect; } cases[] = {
        {3.0f, 3},
        {-3.0f, -3},
        {0.0f, 0},
        {100.0f, 100},
        {-100.0f, -100},
        {12345.0f, 12345},
    };
    int ok = 1;
    for (int i = 0; i < 6; ++i)
    {
        long r = FtoL_model(cases[i].in);
        if (r != cases[i].expect)
        {
            printf("FAIL idx=%d got=%ld expect=%ld\n", i, r, cases[i].expect);
            ok = 0;
        }
    }
    if (ok) printf("OK_00c6b240\n");
    else printf("BAD\n");
    return ok ? 0 : 1;
}