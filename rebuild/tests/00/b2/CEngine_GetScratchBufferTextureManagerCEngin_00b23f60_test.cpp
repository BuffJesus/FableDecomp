#include <cstdio>

struct CGPUScratchBufferTextureManager;
struct CEngine;
char* g_engineSingleton;

CGPUScratchBufferTextureManager* CEngine_GetScratchBufferTextureManager(CEngine* self)
{
    (void)self;
    return (CGPUScratchBufferTextureManager*)(g_engineSingleton + 0x3190);
}

int main()
{
    char base[0x4000];
    g_engineSingleton = base;
    void* r = (void*)CEngine_GetScratchBufferTextureManager((CEngine*)0);
    if (r == (void*)(base + 0x3190)) {
        std::printf("CEngine_00b23f60_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}