#include <cstdio>

struct CAllocator {
    void* Allocate(unsigned int size);
};

static char g_pool[4096];
static unsigned int g_off = 0;

void* CAllocator::Allocate(unsigned int size)
{
    void* p = &g_pool[g_off];
    g_off += size;
    return p;
}

CAllocator g_allocator;

void* __fastcall operator_new(unsigned int size)
{
    return g_allocator.Allocate(size);
}

int main()
{
    void* a = operator_new(16);
    void* b = operator_new(32);
    if (a == 0 || b == 0) { std::printf("FAIL null\n"); return 1; }
    if (a == b) { std::printf("FAIL same\n"); return 1; }
    if ((char*)b - (char*)a != 16) { std::printf("FAIL offset\n"); return 1; }
    std::printf("_global_0044c71f_TEST PASS\n");
    return 0;
}