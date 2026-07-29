#include <stdio.h>

extern "C" long ScaleGlobalIntToInt64_0066fab0_Value = 0;
extern "C" float ScaleGlobalIntToInt64_0066fab0_Scale = 1.0f;

extern "C" __int64 __cdecl ScaleGlobalIntToInt64_0066fab0();

static int Check(__int64 actual, __int64 expected, const char* message)
{
    if (actual != expected)
    {
        printf("FAIL: %s (got %I64d, expected %I64d)\n",
               message, actual, expected);
        return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;

    ScaleGlobalIntToInt64_0066fab0_Value = 11;
    ScaleGlobalIntToInt64_0066fab0_Scale = 1.5f;
    ok &= Check(ScaleGlobalIntToInt64_0066fab0(), 16,
                "positive fractional results must truncate");

    ScaleGlobalIntToInt64_0066fab0_Value = -11;
    ScaleGlobalIntToInt64_0066fab0_Scale = 1.5f;
    ok &= Check(ScaleGlobalIntToInt64_0066fab0(), -16,
                "negative fractional results must truncate toward zero");

    ScaleGlobalIntToInt64_0066fab0_Value = 400000;
    ScaleGlobalIntToInt64_0066fab0_Scale = 10000.0f;
    ok &= Check(ScaleGlobalIntToInt64_0066fab0(), 4000000000I64,
                "conversion must preserve results wider than 32 bits");

    ScaleGlobalIntToInt64_0066fab0_Value = 0;
    ScaleGlobalIntToInt64_0066fab0_Scale = 123.0f;
    ok &= Check(ScaleGlobalIntToInt64_0066fab0(), 0,
                "zero input must remain zero");

    if (!ok)
        return 1;

    puts("PASS scaled global integer conversion 0x0066FAB0");
    return 0;
}
