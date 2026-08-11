#include <cstdio>

static void* g_out;
static unsigned long g_a, g_b;

extern "C" char __cdecl GetCellBoundingBox_callee_005ba9f9(unsigned long a, unsigned long b, void* out)
{
    g_a = a;
    g_b = b;
    g_out = out;
    return 1;
}

char GetCellBoundingBox(unsigned long a, unsigned long b);

int main()
{
    char r = GetCellBoundingBox(0x11u, 0x22u);
    // callee must receive (a=0x11, b=0x22, &local)
    if (r == 1 && g_a == 0x11u && g_b == 0x22u && g_out != 0)
        std::printf("005baeac_TEST PASS\n");
    else
        std::printf("FAIL r=%d a=%lx b=%lx out=%p\n", (int)r, g_a, g_b, g_out);
    return 0;
}