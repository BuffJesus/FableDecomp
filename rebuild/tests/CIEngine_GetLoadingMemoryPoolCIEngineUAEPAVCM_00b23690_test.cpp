#include <cstdio>

struct CMovableResourceMemoryPool;
struct CIEngine;

CMovableResourceMemoryPool* __fastcall CIEngine_GetLoadingMemoryPool(CIEngine* self)
{
    (void)self;
    return 0;
}

int main()
{
    CIEngine* e = (CIEngine*)0x12345678;
    CMovableResourceMemoryPool* r = CIEngine_GetLoadingMemoryPool(e);
    if (r == 0) {
        std::printf("CIEngine_00b23690_TEST PASS\n");
        return 0;
    }
    std::printf("CIEngine_00b23690_TEST FAIL\n");
    return 1;
}