#include <cstdio>

static int g_a, g_b;
static long* g_pv;
static long g_captured;
static void* g_this;

struct CPersistContext {
    void Transfer(int a, int b);
    void TransferImpl(int a, int b, long* pv);
};

void CPersistContext::TransferImpl(int a, int b, long* pv)
{
    g_this = this; g_a = a; g_b = b; g_pv = pv; g_captured = *pv;
}

void CPersistContext::Transfer(int a, int b)
{
    long v = 0;
    TransferImpl(a, b, &v);
}

int main()
{
    CPersistContext ctx;
    ctx.Transfer(7, 9);
    if (g_this == &ctx && g_a == 7 && g_b == 9 && g_pv != 0 && g_captured == 0) {
        std::printf("CPersistContext_00410800_TEST PASS\n");
        return 0;
    }
    std::printf("CPersistContext_00410800_TEST FAIL\n");
    return 1;
}