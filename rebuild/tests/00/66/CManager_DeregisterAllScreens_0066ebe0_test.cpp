#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
CManager_DeregisterAllScreens_0066ebe0(void* manager, void*);

struct Manager
{
    void* world;
    void** screens_begin;
    void** screens_end;
    void** screens_capacity;
};

static int g_initialise_count;
static void* g_initialise_manager;
static long g_initialise_value;

extern "C" void __fastcall
CManager_DeregisterAllScreens_0066ebe0_InitialiseScrollIndices(
    void* manager, void*, long count)
{
    ++g_initialise_count;
    g_initialise_manager = manager;
    g_initialise_value = count;
}

static void ResetCalls()
{
    g_initialise_count = 0;
    g_initialise_manager = 0;
    g_initialise_value = -1;
}

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
    void* screens[5];

    screens[0] = (void*)0x11111111;
    screens[1] = 0;
    screens[2] = (void*)0x22222222;
    screens[3] = (void*)0x33333333;
    screens[4] = 0;
    manager.world = (void*)0x44444444;
    manager.screens_begin = screens;
    manager.screens_end = screens + 5;
    manager.screens_capacity = screens + 5;
    ResetCalls();
    CManager_DeregisterAllScreens_0066ebe0(&manager, 0);
    ok &= Check(screens[0] == 0 &&
                screens[1] == 0 &&
                screens[2] == 0 &&
                screens[3] == 0 &&
                screens[4] == 0,
                "every registration slot must be cleared");
    ok &= Check(manager.world == (void*)0x44444444,
                "unrelated manager state must be preserved");
    ok &= Check(g_initialise_count == 1 &&
                g_initialise_manager == &manager &&
                g_initialise_value == 0,
                "scroll indices must be reset to zero entries");

    manager.screens_end = manager.screens_begin;
    ResetCalls();
    CManager_DeregisterAllScreens_0066ebe0(&manager, 0);
    ok &= Check(g_initialise_count == 1 &&
                g_initialise_manager == &manager &&
                g_initialise_value == 0,
                "empty registration vector must still reset scroll indices");

    if (!ok)
        return 1;

    puts("PASS CManager::DeregisterAllScreens");
    return 0;
}
