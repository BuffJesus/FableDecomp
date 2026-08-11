#include <stdio.h>

typedef unsigned __int64 uint64;
extern "C" unsigned __int64 __cdecl CPUAheadCounterCallback(unsigned long unused);

int main()
{
    uint64 a = CPUAheadCounterCallback(0);
    uint64 b = CPUAheadCounterCallback(0);
    // rdtsc is monotonic non-decreasing across two back-to-back calls, and nonzero.
    if (a != 0 && b >= a) {
        printf("RDTSC_OK\n");
        return 0;
    }
    printf("RDTSC_FAIL a=%llu b=%llu\n", a, b);
    return 1;
}