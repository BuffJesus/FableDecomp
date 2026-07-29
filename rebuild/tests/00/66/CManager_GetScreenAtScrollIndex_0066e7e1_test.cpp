#include <stdio.h>
#include <string.h>

extern "C" void* __fastcall
CManager_GetScreenAtScrollIndex_0066e7e1(void* manager, void*, long scroll_index);

struct Manager
{
    unsigned char prefix[0x10];
    void** screens_begin;
    void** screens_end;
    void** screens_capacity;
    unsigned long* scroll_indices;
};

static int Check(int condition, const char* message)
{
    if (!condition)
    {
        printf("FAIL: %s\n", message);
        return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    Manager manager;
    void* screens[3];
    unsigned long scroll_indices[5];

    memset(&manager, 0xCC, sizeof(manager));
    screens[0] = (void*)0x11111111;
    screens[1] = (void*)0x22222222;
    screens[2] = (void*)0x33333333;
    scroll_indices[0] = 2;
    scroll_indices[1] = 0;
    scroll_indices[2] = 1;
    scroll_indices[3] = 3;
    scroll_indices[4] = 0xFFFFFFFF;
    manager.screens_begin = screens;
    manager.screens_end = screens + 3;
    manager.screens_capacity = screens + 3;
    manager.scroll_indices = scroll_indices;

    ok &= Check(CManager_GetScreenAtScrollIndex_0066e7e1(&manager, 0, 0) ==
                    screens[2],
                "scroll slot zero must map through the index array");
    ok &= Check(CManager_GetScreenAtScrollIndex_0066e7e1(&manager, 0, 1) ==
                    screens[0],
                "scroll slot one must map independently of slot order");
    ok &= Check(CManager_GetScreenAtScrollIndex_0066e7e1(&manager, 0, 2) ==
                    screens[1],
                "last valid mapped index must return its screen");
    ok &= Check(CManager_GetScreenAtScrollIndex_0066e7e1(&manager, 0, 3) == 0,
                "mapped index equal to screen count must return null");
    ok &= Check(CManager_GetScreenAtScrollIndex_0066e7e1(&manager, 0, 4) == 0,
                "large unsigned mapped index must return null");

    manager.screens_end = manager.screens_begin;
    ok &= Check(CManager_GetScreenAtScrollIndex_0066e7e1(&manager, 0, 1) == 0,
                "empty screen vector must reject every mapped index");

    if (!ok)
        return 1;

    puts("PASS CManager::GetScreenAtScrollIndex");
    return 0;
}
