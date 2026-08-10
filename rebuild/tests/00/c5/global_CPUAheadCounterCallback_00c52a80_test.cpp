#include <stdio.h>

extern "C" unsigned __int64 __cdecl CPUAheadCounterCallback(void);

int main(void)
{
    unsigned __int64 a = CPUAheadCounterCallback();
    unsigned __int64 b = CPUAheadCounterCallback();
    /* rdtsc is monotonic non-decreasing over back-to-back calls on one core */
    if (b >= a && a != 0)
        printf("00c52a80_TEST PASS\n");
    else
        printf("00c52a80_TEST FAIL a=%I64u b=%I64u\n", a, b);
    return 0;
}