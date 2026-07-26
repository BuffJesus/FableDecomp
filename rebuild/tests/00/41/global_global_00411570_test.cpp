#include <cstdio>

static long strcmp_signed(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

int main()
{
    if (strcmp_signed("abc", "abc") != 0) { std::printf("FAIL eq\n"); return 1; }
    if (strcmp_signed("abc", "abd") != -1) { std::printf("FAIL lt\n"); return 1; }
    if (strcmp_signed("abd", "abc") != 1) { std::printf("FAIL gt\n"); return 1; }
    if (strcmp_signed("ab", "abc") != -1) { std::printf("FAIL short\n"); return 1; }
    if (strcmp_signed("", "") != 0) { std::printf("FAIL empty\n"); return 1; }
    std::printf("_global_00411570_TEST PASS\n");
    return 0;
}