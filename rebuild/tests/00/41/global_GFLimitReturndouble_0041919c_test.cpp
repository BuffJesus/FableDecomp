#include <stdio.h>
#include "fable_game.h"

static bool returns(const double& value, const double& minimum,
                    const double& maximum, const double* expected)
{
    return &GFLimitReturn(value, minimum, maximum) == expected;
}

int main()
{
    double low = -2.0, middle = 1.0, high = 8.0;
    double below = -4.0, above = 12.0, equalLow = -2.0, equalHigh = 8.0;
    double negativeZero = -0.0, positiveZero = 0.0;
    volatile unsigned long nanBits[2] = {0, 0x7ff80000};
    const double& nan = *reinterpret_cast<const double*>(
        const_cast<const unsigned long*>(nanBits));

    if (!returns(below, low, high, &low)) return 1;
    if (!returns(middle, low, high, &middle)) return 2;
    if (!returns(above, low, high, &high)) return 3;
    if (!returns(equalLow, low, high, &equalLow)) return 4;
    if (!returns(equalHigh, low, high, &equalHigh)) return 5;
    if (!returns(middle, high, low, &high)) return 6;
    if (!returns(nan, low, high, &nan)) return 7;
    if (!returns(middle, nan, high, &middle)) return 8;
    if (!returns(middle, low, nan, &middle)) return 9;
    if (!returns(negativeZero, positiveZero, high, &negativeZero)) return 10;
    if (!returns(middle, middle, middle, &middle)) return 11;

    puts("LIMIT_RETURN_DOUBLE_PASS");
    return 0;
}
