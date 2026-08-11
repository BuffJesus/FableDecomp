#include <cstdio>

struct CHandleBasedMemoryPool
{
    void MoveResource(void *p0, long p1, void *p2, void *p3, unsigned long p4);
};

static void *g_dst;
static void *g_mid;
static unsigned long g_n;

extern "C" void __cdecl MoveResourceHelper(void *dst, void *mid, unsigned long n)
{
    g_dst = dst;
    g_mid = mid;
    g_n = n;
}

void CHandleBasedMemoryPool::MoveResource(void *p0, long p1, void *p2, void *p3, unsigned long p4)
{
    MoveResourceHelper(p3, p2, p4);
}

int main()
{
    CHandleBasedMemoryPool pool;
    int a, b, c, d;
    void *p0 = &a;
    void *p2 = &b;
    void *p3 = &c;
    (void)d;
    pool.MoveResource(p0, 77, p2, p3, 12345UL);

    bool ok = (g_dst == p3) && (g_mid == p2) && (g_n == 12345UL);
    if (ok)
        printf("MOVERESOURCE_OK\n");
    else
        printf("MOVERESOURCE_FAIL dst=%p mid=%p n=%lu\n", g_dst, g_mid, g_n);
    return ok ? 0 : 1;
}