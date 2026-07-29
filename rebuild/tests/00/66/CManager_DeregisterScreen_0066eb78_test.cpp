#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
CManager_DeregisterScreen_0066eb78(void* manager, void*, void* screen);

extern "C" unsigned char FableDeregisterScreenGuard = 0;

struct Screen;
typedef long (__fastcall* GetIndexFunction)(Screen*, void*);

struct ScreenVtable
{
    void* slots[2];
    GetIndexFunction get_index;
};

struct Screen
{
    ScreenVtable* vtable;
    long index;
};

struct Manager
{
    void* world;
    Screen** screens_begin;
    Screen** screens_end;
    Screen** screens_capacity;
    void** inventory_begin;
    void** inventory_end;
    void** inventory_capacity;
    long* scroll_begin;
    long* scroll_end;
    long* scroll_capacity;
};

static int g_index_count;
static int g_initialise_count;
static void* g_index_this;
static void* g_initialise_manager;
static long g_initialise_value;

static long __fastcall GetIndex(Screen* screen, void*)
{
    ++g_index_count;
    g_index_this = screen;
    return screen->index;
}

static void ResetCalls()
{
    g_index_count = 0;
    g_initialise_count = 0;
    g_index_this = 0;
    g_initialise_manager = 0;
    g_initialise_value = -1;
}

extern "C" void __fastcall
CManager_DeregisterScreen_0066eb78_InitialiseScrollIndices(
    void* manager, void*, long count)
{
    ++g_initialise_count;
    g_initialise_manager = manager;
    g_initialise_value = count;
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

static void SetupManager(
    Manager& manager, Screen** screens, int screen_count,
    long* scroll, int scroll_count)
{
    memset(&manager, 0, sizeof(manager));
    manager.screens_begin = screens;
    manager.screens_end = screens + screen_count;
    manager.screens_capacity = screens + screen_count;
    manager.scroll_begin = scroll;
    manager.scroll_end = scroll + scroll_count;
    manager.scroll_capacity = scroll + 8;
}

int main()
{
    int ok = 1;
    Manager manager;
    ScreenVtable vtable;
    Screen target;
    Screen other0;
    Screen other1;
    Screen* screens[6];
    long scroll[8];

    memset(&vtable, 0, sizeof(vtable));
    vtable.get_index = GetIndex;
    target.vtable = &vtable;
    target.index = 1;
    other0.vtable = &vtable;
    other0.index = 0;
    other1.vtable = &vtable;
    other1.index = 2;

    memset(screens, 0, sizeof(screens));
    screens[0] = &other0;
    screens[1] = &target;
    SetupManager(manager, screens, 4, scroll, 4);
    FableDeregisterScreenGuard = 1;
    ResetCalls();
    CManager_DeregisterScreen_0066eb78(&manager, 0, &target);
    ok &= Check(g_index_count == 0 &&
                g_initialise_count == 0 &&
                screens[1] == &target,
                "active guard must suppress all deregistration work");

    FableDeregisterScreenGuard = 0;
    screens[0] = &other0;
    screens[1] = &target;
    screens[2] = &other1;
    screens[3] = 0;
    SetupManager(manager, screens, 4, scroll, 4);
    ResetCalls();
    CManager_DeregisterScreen_0066eb78(&manager, 0, &target);
    ok &= Check(g_index_count == 1 &&
                g_index_this == &target &&
                screens[1] == 0,
                "matching virtual slot must be cleared");
    ok &= Check(g_initialise_count == 1 &&
                g_initialise_manager == &manager &&
                g_initialise_value == 2,
                "smaller registered count must rebuild scroll indices");

    target.index = 1;
    screens[0] = &other0;
    screens[1] = &other1;
    screens[2] = 0;
    SetupManager(manager, screens, 3, scroll, 2);
    ResetCalls();
    CManager_DeregisterScreen_0066eb78(&manager, 0, &target);
    ok &= Check(screens[1] == &other1,
                "mismatched pointer in virtual slot must be preserved");
    ok &= Check(g_initialise_count == 0,
                "registered count equal to scroll count must not rebuild");

    target.index = 1;
    screens[0] = &other0;
    screens[1] = &target;
    screens[2] = &other1;
    SetupManager(manager, screens, 3, scroll, 1);
    ResetCalls();
    CManager_DeregisterScreen_0066eb78(&manager, 0, &target);
    ok &= Check(screens[1] == 0,
                "matching slot must clear even without rebuild");
    ok &= Check(g_initialise_count == 0,
                "registered count above scroll count must not rebuild");

    if (!ok)
        return 1;

    puts("PASS CManager::DeregisterScreen");
    return 0;
}
