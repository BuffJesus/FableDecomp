#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CManager_Update_0066e8ee(void* manager, void*);

struct Manager
{
    unsigned char prefix[0x10];
    void** screens_begin;
    void** screens_end;
    void** screens_capacity;
    long* scroll_begin;
    long* scroll_end;
    long* scroll_capacity;
    long requested_index;
    long current_index;
};

static int g_next_count;
static int g_previous_count;
static void* g_called_manager;
static long g_scroll_count;
static long g_screen_count;

static void ResetCalls()
{
    g_next_count = 0;
    g_previous_count = 0;
    g_called_manager = 0;
    g_scroll_count = -1;
    g_screen_count = -1;
}

extern "C" void __fastcall
CManager_ProcessNextScreenAction_0066e845(
    void* manager, void*, long scroll_count, long screen_count)
{
    ++g_next_count;
    g_called_manager = manager;
    g_scroll_count = scroll_count;
    g_screen_count = screen_count;
}

extern "C" void __fastcall
CManager_ProcessPreviousScreenAction_0066e8a1(
    void* manager, void*, long scroll_count, long screen_count)
{
    ++g_previous_count;
    g_called_manager = manager;
    g_scroll_count = scroll_count;
    g_screen_count = screen_count;
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

static int CheckDispatch(
    const Manager& manager, int next_count, int previous_count, long expected_index)
{
    int ok = 1;
    ok &= Check(g_next_count == next_count &&
                g_previous_count == previous_count,
                "navigation direction must match");
    ok &= Check(manager.current_index == expected_index,
                "requested index must be committed");
    if (next_count || previous_count)
    {
        ok &= Check(g_called_manager == &manager,
                    "navigation helper must receive the manager");
        ok &= Check(g_scroll_count == 2 && g_screen_count == 4,
                    "navigation helper must receive both derived vector counts");
    }
    return ok;
}

int main()
{
    int ok = 1;
    Manager manager;
    void* screens[4];
    long scroll_indices[2];

    memset(&manager, 0, sizeof(manager));
    manager.screens_begin = screens;
    manager.screens_end = screens + 4;
    manager.screens_capacity = screens + 4;
    manager.scroll_begin = scroll_indices;
    manager.scroll_end = scroll_indices + 2;
    manager.scroll_capacity = scroll_indices + 2;

    manager.current_index = 2;
    manager.requested_index = 2;
    ResetCalls();
    CManager_Update_0066e8ee(&manager, 0);
    ok &= CheckDispatch(manager, 0, 0, 2);

    manager.current_index = 1;
    manager.requested_index = 3;
    ResetCalls();
    CManager_Update_0066e8ee(&manager, 0);
    ok &= CheckDispatch(manager, 1, 0, 3);

    manager.current_index = 3;
    manager.requested_index = 1;
    ResetCalls();
    CManager_Update_0066e8ee(&manager, 0);
    ok &= CheckDispatch(manager, 0, 1, 1);

    manager.current_index = 4;
    manager.requested_index = 0;
    ResetCalls();
    CManager_Update_0066e8ee(&manager, 0);
    ok &= CheckDispatch(manager, 1, 0, 0);

    manager.current_index = 0;
    manager.requested_index = 4;
    ResetCalls();
    CManager_Update_0066e8ee(&manager, 0);
    ok &= CheckDispatch(manager, 0, 1, 4);

    if (!ok)
        return 1;

    puts("PASS CManager::Update");
    return 0;
}
