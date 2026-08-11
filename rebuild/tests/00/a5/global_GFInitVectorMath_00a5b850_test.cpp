#include <cstdio>

extern unsigned char g_HasSSE;
void GFInitVectorMath();

int main()
{
    g_HasSSE = 0;
    GFInitVectorMath();
    // On any modern x86 CPU running this test, SSE is present, so the flag
    // must be set to exactly 1. If the CPU lacked SSE it would stay 0.
    if (g_HasSSE == 1)
    {
        printf("VECTORMATH_SSE_OK\n");
        return 0;
    }
    printf("VECTORMATH_SSE_FAIL flag=%d\n", (int)g_HasSSE);
    return 1;
}