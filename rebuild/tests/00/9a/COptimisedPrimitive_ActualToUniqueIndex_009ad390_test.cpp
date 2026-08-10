#include <cstdio>

struct SubTable {
    char pad[8];
    long* data;   // +8
};

static long g_arr[4] = { 10, 20, 30, 40 };
static SubTable g_tab;

extern "C" SubTable* __stdcall sub_9ad2e0(unsigned long a)
{
    g_tab.data = g_arr;
    (void)a;
    return &g_tab;
}

struct COptimisedPrimitive;
long __fastcall ActualToUniqueIndex(COptimisedPrimitive* self, int e, unsigned long a, unsigned long b);

int main()
{
    long r = ActualToUniqueIndex((COptimisedPrimitive*)0, 0, /*a*/1u, /*b*/2u);
    if (r == 30) printf("009ad390_TEST PASS\n");
    else printf("FAIL r=%ld\n", r);
    return 0;
}