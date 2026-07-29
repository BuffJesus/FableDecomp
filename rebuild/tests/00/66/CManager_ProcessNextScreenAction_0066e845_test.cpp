#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
CManager_ProcessNextScreenAction_0066e845(
    void* manager, void*, long screen_count, long scroll_count);

struct Screen;
typedef unsigned char (__fastcall* ScreenAction)(Screen*, void*);

struct ScreenVtable
{
    void* slots[3];
    ScreenAction process_next;
};

struct Screen
{
    ScreenVtable* vtable;
    int identifier;
};

struct Manager
{
    unsigned char prefix0[0x10];
    Screen** screens_begin;
    Screen** screens_end;
    Screen** screens_capacity;
    unsigned long* scroll_indices;
    unsigned char prefix20[0x10];
    long current_action;
};

extern "C" void* __fastcall
CManager_GetScreenAtScrollIndex_0066e7e1(
    void* raw_manager, void*, long scroll_index)
{
    Manager* manager = static_cast<Manager*>(raw_manager);
    return manager->screens_begin[
        manager->scroll_indices[scroll_index]];
}

static int g_call_count;
static int g_call_order[16];
static int g_false_calls_remaining;

static unsigned char __fastcall ProcessNext(Screen* screen, void*)
{
    g_call_order[g_call_count++] = screen->identifier;
    if (g_false_calls_remaining > 0)
    {
        --g_false_calls_remaining;
        return 0;
    }
    return 1;
}

static void ResetCalls(int false_calls)
{
    g_call_count = 0;
    memset(g_call_order, 0, sizeof(g_call_order));
    g_false_calls_remaining = false_calls;
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
    ScreenVtable vtable;
    Screen screen0;
    Screen screen1;
    Screen screen2;
    Screen* screens[3];
    unsigned long scroll_indices[3];

    memset(&manager, 0, sizeof(manager));
    memset(&vtable, 0, sizeof(vtable));
    vtable.process_next = ProcessNext;
    screen0.vtable = &vtable;
    screen0.identifier = 0;
    screen1.vtable = &vtable;
    screen1.identifier = 1;
    screen2.vtable = &vtable;
    screen2.identifier = 2;
    screens[0] = &screen0;
    screens[1] = &screen1;
    screens[2] = &screen2;
    manager.screens_begin = screens;
    manager.screens_end = screens + 3;
    manager.screens_capacity = screens + 3;
    manager.scroll_indices = scroll_indices;

    scroll_indices[0] = 0;
    scroll_indices[1] = 1;
    scroll_indices[2] = 2;
    manager.current_action = 0;
    ResetCalls(0);
    CManager_ProcessNextScreenAction_0066e845(&manager, 0, 3, 3);
    ok &= Check(manager.current_action == 1,
                "non-boundary call must increment the current action");
    ok &= Check(scroll_indices[0] == 0 &&
                scroll_indices[1] == 1 &&
                scroll_indices[2] == 2,
                "non-boundary call must preserve scroll indices");
    ok &= Check(g_call_count == 0,
                "non-boundary call must not process screens");

    scroll_indices[0] = 0;
    scroll_indices[1] = 2;
    scroll_indices[2] = 1;
    manager.current_action = 2;
    ResetCalls(0);
    CManager_ProcessNextScreenAction_0066e845(&manager, 0, 3, 3);
    ok &= Check(manager.current_action == 2,
                "boundary call must clamp current action to the last slot");
    ok &= Check(scroll_indices[0] == 1 &&
                scroll_indices[1] == 0 &&
                scroll_indices[2] == 2,
                "boundary call must advance and wrap every scroll index");
    ok &= Check(g_call_count == 3 &&
                g_call_order[0] == 1 &&
                g_call_order[1] == 0 &&
                g_call_order[2] == 2,
                "boundary call must process the newly selected screens");

    scroll_indices[0] = 0;
    manager.current_action = 0;
    ResetCalls(1);
    CManager_ProcessNextScreenAction_0066e845(&manager, 0, 1, 3);
    ok &= Check(manager.current_action == 0,
                "single action must remain at index zero");
    ok &= Check(scroll_indices[0] == 2,
                "rejected screen must advance again until accepted");
    ok &= Check(g_call_count == 2 &&
                g_call_order[0] == 1 &&
                g_call_order[1] == 2,
                "screen callback rejection must retry the same action");

    scroll_indices[0] = 1;
    manager.current_action = 5;
    ResetCalls(0);
    CManager_ProcessNextScreenAction_0066e845(&manager, 0, 0, 3);
    ok &= Check(manager.current_action == -1,
                "zero screen count must set the last-index sentinel");
    ok &= Check(scroll_indices[0] == 1 && g_call_count == 0,
                "zero screen count must skip the processing loop");

    if (!ok)
        return 1;

    puts("PASS CManager::ProcessNextScreenAction");
    return 0;
}
