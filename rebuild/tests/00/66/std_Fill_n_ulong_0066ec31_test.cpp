#include <stdio.h>

extern "C" void __fastcall
std_Fill_n_ulong_0066ec31(
    unsigned long* destination, unsigned long count, const unsigned long* value);

static int Check(int condition, const char* message)
{
    if (!condition)
    {
        printf("FAIL: %s\n", message);
        return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    unsigned long values[7];
    unsigned long fill;
    int i;

    for (i = 0; i < 7; ++i)
        values[i] = 0x10000000 + i;
    fill = 0xA5A5A5A5;
    std_Fill_n_ulong_0066ec31(values + 1, 0, &fill);
    for (i = 0; i < 7; ++i)
    {
        ok &= Check(values[i] == 0x10000000 + i,
                    "zero count must preserve every value");
    }

    std_Fill_n_ulong_0066ec31(values + 2, 4, &fill);
    ok &= Check(values[0] == 0x10000000 &&
                values[1] == 0x10000001,
                "fill must preserve the prefix");
    ok &= Check(values[2] == fill &&
                values[3] == fill &&
                values[4] == fill &&
                values[5] == fill,
                "fill must write exactly count elements");
    ok &= Check(values[6] == 0x10000006,
                "fill must preserve the suffix");

    fill = 0x12345678;
    std_Fill_n_ulong_0066ec31(values + 6, 1, &fill);
    ok &= Check(values[6] == fill,
                "single-element fill must write one value");

    if (!ok)
        return 1;

    puts("PASS std::_Fill_n<unsigned long>");
    return 0;
}
