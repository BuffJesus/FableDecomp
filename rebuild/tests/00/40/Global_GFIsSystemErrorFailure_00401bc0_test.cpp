#include "fable_system.h"
#include <stdio.h>
bool FABLE_FASTCALL GFIsSystemErrorFailure(ESystemCode);
// Expected dispatch bytes read from retail 00401BE4, not from candidate C++.
static const unsigned char retailDispatch[14] = {0,1,0,0,1,1,1,1,0,1,0,0,0,0};
static int Check(unsigned long value)
{
    unsigned long index = value - 2;
    bool expected = index < 14 && retailDispatch[index] == 0;
    bool actual = GFIsSystemErrorFailure(static_cast<ESystemCode>(value));
    if (expected != actual)
    {
        printf("FAIL code=%08lx\n", value);
        return 1;
    }
    return 0;
}
int main()
{
    unsigned long cases = 0;
    for (unsigned long value = 0; value < 65536; ++value)
    {
        if (Check(value) || Check(0xffff0000UL + value)) return 1;
        cases += 2;
    }
    for (unsigned int bit = 0; bit < 32; ++bit)
    {
        unsigned long value = 1UL << bit;
        if (Check(value - 1) || Check(value) || Check(value + 1)) return 1;
        cases += 3;
    }
    printf("SYSTEM_ERROR_PREDICATE_PASS cases=%lu\n", cases);
    return 0;
}
