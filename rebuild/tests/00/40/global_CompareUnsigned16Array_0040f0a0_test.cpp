#include <stdio.h>

static int Compare(const unsigned short* lhs, int lhs_count, const unsigned short* rhs, int rhs_count)
{
    if (lhs_count != rhs_count) return 0;
    for (int i = 0; i != lhs_count; ++i) {
        if (lhs[i] != rhs[i]) return 0;
    }
    return 1;
}

int main()
{
    unsigned short a[4] = { 1, 2, 65535, 9 };
    unsigned short b[4] = { 1, 2, 65535, 9 };
    unsigned short c[4] = { 1, 2, 7, 9 };
    if (!Compare(a, 4, b, 4)) return 1;
    if (Compare(a, 4, c, 4)) return 2;
    if (Compare(a, 3, b, 4)) return 3;
    if (!Compare(a, 0, b, 0)) return 4;
    puts("COMPARE_U16_PASS");
    return 0;
}