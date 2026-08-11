#include <cstdio>

static unsigned long g_a, g_b;
static char g_ret;

extern "C" char __cdecl helper_5b155a(unsigned long a, unsigned long b, char* out)
{
    g_a = a;
    g_b = b;
    *out = g_ret;
    return g_ret;
}

char __cdecl GetCellBoundingBox(unsigned long a, unsigned long b);

int main()
{
    g_ret = 1;
    char r1 = GetCellBoundingBox(7u, 99u);
    bool okargs = (g_a == 7u && g_b == 99u);
    g_ret = 0;
    char r2 = GetCellBoundingBox(3u, 4u);

    if (r1 == 1 && r2 == 0 && okargs)
        std::printf("005b1975_TEST PASS\n");
    else
        std::printf("FAIL r1=%d r2=%d args=%d\n", (int)r1, (int)r2, (int)okargs);
    return 0;
}