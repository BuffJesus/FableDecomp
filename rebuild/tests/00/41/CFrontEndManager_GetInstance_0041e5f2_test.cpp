#include <stdio.h>
#include <string.h>

extern "C" void* CFrontEndManager_GetInstance_0041e5f2();

static void* g_managerInstance;
static unsigned char g_managerStorage[0xd0];
static int g_allocateCount;
static int g_constructCount;

extern "C" void* FableFrontEndManagerInstance = 0;

extern "C" void* FableFrontEndManagerAllocate(unsigned long size)
{
    if (size != 0xd0)
        return 0;
    ++g_allocateCount;
    return g_managerStorage;
}

extern "C" void* __fastcall FableFrontEndManagerConstruct(void* manager)
{
    ++g_constructCount;
    g_managerInstance = manager;
    return manager;
}

int main()
{
    void* first = CFrontEndManager_GetInstance_0041e5f2();
    void* second = CFrontEndManager_GetInstance_0041e5f2();

    if (first != g_managerStorage || second != g_managerStorage)
        return 1;
    if (g_managerInstance != g_managerStorage)
        return 2;
    if (g_allocateCount != 1 || g_constructCount != 1)
        return 3;

    puts("CFrontEndManager_GetInstance_0041e5f2_test PASS");
    return 0;
}
