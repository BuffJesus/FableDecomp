#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
CManager_RemoveAllPlayerModes_0066e985(void* manager, void*);

struct PointerVector
{
    void** begin;
    void** end;
    void** capacity;
};

struct Screen;
typedef long (__fastcall* GetModeFunction)(Screen*, void*);

struct ScreenVtable
{
    void* slot0;
    GetModeFunction get_mode;
};

struct Screen
{
    ScreenVtable* vtable;
    long mode;
};

struct Root
{
    unsigned char prefix[0x1C];
    void* player_context;
};

static Root g_root;
extern "C" void* FableRemoveAllPlayerModesGlobal = &g_root;

static void* g_player = (void*)0x11111111;
static Screen** g_screens_begin;
static Screen** g_screens_end;
static int g_get_player_count;
static int g_get_screens_count;
static int g_remove_count;
static int g_free_count;
static void* g_get_player_context;
static void* g_get_screens_manager;
static void* g_removed_player[8];
static long g_removed_modes[8];
static void* g_freed_memory;

static long __fastcall GetMode(Screen* screen, void*)
{
    return screen->mode;
}

static void ResetCalls()
{
    g_get_player_count = 0;
    g_get_screens_count = 0;
    g_remove_count = 0;
    g_free_count = 0;
    g_get_player_context = 0;
    g_get_screens_manager = 0;
    memset(g_removed_player, 0, sizeof(g_removed_player));
    memset(g_removed_modes, 0, sizeof(g_removed_modes));
    g_freed_memory = 0;
}

extern "C" void* __fastcall
CManager_RemoveAllPlayerModes_0066e985_GetPlayer(void* context, void*)
{
    ++g_get_player_count;
    g_get_player_context = context;
    return g_player;
}

extern "C" void __fastcall
CManager_RemoveAllPlayerModes_0066e985_GetScreens(
    void* manager, void*, void* output)
{
    PointerVector* vector = (PointerVector*)output;
    ++g_get_screens_count;
    g_get_screens_manager = manager;
    vector->begin = (void**)g_screens_begin;
    vector->end = (void**)g_screens_end;
    vector->capacity = (void**)g_screens_end;
}

extern "C" void __fastcall
CManager_RemoveAllPlayerModes_0066e985_RemoveMode(
    void* player, void*, long mode)
{
    g_removed_player[g_remove_count] = player;
    g_removed_modes[g_remove_count] = mode;
    ++g_remove_count;
}

extern "C" void __cdecl
CManager_RemoveAllPlayerModes_0066e985_Free(void* memory)
{
    ++g_free_count;
    g_freed_memory = memory;
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
    int manager_token = 0;
    ScreenVtable vtable;
    Screen screen0;
    Screen screen1;
    Screen screen2;
    Screen* screens[3];

    vtable.slot0 = 0;
    vtable.get_mode = GetMode;
    screen0.vtable = &vtable;
    screen0.mode = 7;
    screen1.vtable = &vtable;
    screen1.mode = 19;
    screen2.vtable = &vtable;
    screen2.mode = 31;
    screens[0] = &screen0;
    screens[1] = &screen1;
    screens[2] = &screen2;
    g_root.player_context = (void*)0x22222222;

    g_screens_begin = screens;
    g_screens_end = screens + 3;
    ResetCalls();
    CManager_RemoveAllPlayerModes_0066e985(&manager_token, 0);
    ok &= Check(g_get_player_count == 1 &&
                g_get_player_context == g_root.player_context,
                "active player must be resolved through the global context");
    ok &= Check(g_get_screens_count == 1 &&
                g_get_screens_manager == &manager_token,
                "registered screens must be gathered from this manager");
    ok &= Check(g_remove_count == 3 &&
                g_removed_modes[0] == 7 &&
                g_removed_modes[1] == 19 &&
                g_removed_modes[2] == 31,
                "every gathered screen mode must be removed in order");
    ok &= Check(g_removed_player[0] == g_player &&
                g_removed_player[1] == g_player &&
                g_removed_player[2] == g_player,
                "all modes must be removed from the resolved player");
    ok &= Check(g_free_count == 1 && g_freed_memory == screens,
                "populated temporary screen storage must be freed");

    g_screens_begin = 0;
    g_screens_end = 0;
    ResetCalls();
    CManager_RemoveAllPlayerModes_0066e985(&manager_token, 0);
    ok &= Check(g_get_player_count == 1 && g_get_screens_count == 1,
                "empty path must still resolve player and gather screens");
    ok &= Check(g_remove_count == 0 && g_free_count == 0,
                "null empty storage must neither remove modes nor free");

    g_screens_begin = screens;
    g_screens_end = screens;
    ResetCalls();
    CManager_RemoveAllPlayerModes_0066e985(&manager_token, 0);
    ok &= Check(g_remove_count == 0,
                "non-null empty storage must not remove modes");
    ok &= Check(g_free_count == 1 && g_freed_memory == screens,
                "non-null empty storage must still be freed");

    if (!ok)
        return 1;

    puts("PASS CManager::RemoveAllPlayerModes");
    return 0;
}
