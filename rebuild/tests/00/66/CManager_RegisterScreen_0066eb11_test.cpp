#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
CManager_RegisterScreen_0066eb11(void* manager, void*, void* screen);

extern "C" unsigned char FableRegisterScreenGuard = 0;

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
CManager_RegisterScreen_0066eb11_InitialiseScrollIndices(
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
    Screen screen;
    Screen other0;
    Screen other1;
    Screen* screens[8];
    long scroll[8];

    memset(&vtable, 0, sizeof(vtable));
    vtable.get_index = GetIndex;
    screen.vtable = &vtable;
    screen.index = 2;
    other0.vtable = &vtable;
    other0.index = 0;
    other1.vtable = &vtable;
    other1.index = 1;

    memset(screens, 0, sizeof(screens));
    SetupManager(manager, screens, 6, scroll, 0);
    FableRegisterScreenGuard = 1;
    ResetCalls();
    CManager_RegisterScreen_0066eb11(&manager, 0, &screen);
    ok &= Check(g_index_count == 0 &&
                g_initialise_count == 0 &&
                screens[2] == 0,
                "active guard must suppress all registration work");

    memset(screens, 0, sizeof(screens));
    SetupManager(manager, screens, 6, scroll, 5);
    FableRegisterScreenGuard = 0;
    ResetCalls();
    CManager_RegisterScreen_0066eb11(&manager, 0, &screen);
    ok &= Check(g_index_count == 1 &&
                g_index_this == &screen &&
                screens[2] == &screen,
                "screen must be stored at its virtual index");
    ok &= Check(g_initialise_count == 0,
                "five existing scroll indices must suppress rebuilding");

    memset(screens, 0, sizeof(screens));
    screens[0] = &other0;
    screens[1] = &other1;
    screen.index = 4;
    SetupManager(manager, screens, 6, scroll, 2);
    ResetCalls();
    CManager_RegisterScreen_0066eb11(&manager, 0, &screen);
    ok &= Check(screens[4] == &screen,
                "registration must update the requested slot");
    ok &= Check(g_initialise_count == 1 &&
                g_initialise_manager == &manager &&
                g_initialise_value == 3,
                "small registration set must rebuild using non-null count");

    memset(screens, 0, sizeof(screens));
    screens[0] = &other0;
    screens[1] = &other1;
    screens[2] = &other0;
    screens[3] = &other1;
    screens[4] = &other0;
    screen.index = 5;
    SetupManager(manager, screens, 7, scroll, 1);
    ResetCalls();
    CManager_RegisterScreen_0066eb11(&manager, 0, &screen);
    ok &= Check(screens[5] == &screen,
                "large registration set must still store the screen");
    ok &= Check(g_initialise_count == 0,
                "more than five registered screens must suppress rebuilding");

    if (!ok)
        return 1;

    puts("PASS CManager::RegisterScreen");
    return 0;
}
