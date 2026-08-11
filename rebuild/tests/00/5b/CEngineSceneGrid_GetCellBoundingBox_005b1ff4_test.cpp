#include <cstdio>

static void* g_seen_out;
static unsigned long g_seen_a, g_seen_b;

bool helper_5b1afd(unsigned long a, unsigned long b, void* out)
{
    g_seen_a = a; g_seen_b = b; g_seen_out = out;
    return (a + b) == 7;
}

bool __cdecl GetCellBoundingBox(unsigned long a, unsigned long b);

int main()
{
    bool r = GetCellBoundingBox(3, 4);
    bool r2 = GetCellBoundingBox(1, 1);
    if (r && !r2 && g_seen_a == 1 && g_seen_b == 1 && g_seen_out != 0)
        printf("005b1ff4_TEST PASS\n");
    else
        printf("FAIL r=%d r2=%d a=%lu b=%lu\n", r, r2, g_seen_a, g_seen_b);
    return 0;
}