#include <stdio.h>
#include <string.h>

extern "C" void* __fastcall
CManager_CManager_0066eab7(void* manager, void*, void* world);

struct Manager
{
    void* world;
    unsigned long screens[3];
    unsigned long state[9];
    unsigned char open;
    unsigned char padding35[3];
    unsigned char scene[0x20];
};

static int g_sequence;
static int g_screens_sequence;
static int g_scene_sequence;
static int g_indices_sequence;
static void* g_screens_this;
static long g_screens_count;
static unsigned long g_seed_value;
static long g_seed_span;
static void* g_scene_this;
static void* g_indices_manager;
static long g_indices_count;
static int g_indices_saw_zero_state;

extern "C" void __fastcall
CManager_CManager_0066eab7_ConstructScreens(
    void* screens, void*, long count, void* value, void* value_end)
{
    unsigned long* fields = (unsigned long*)screens;
    g_screens_sequence = ++g_sequence;
    g_screens_this = screens;
    g_screens_count = count;
    g_seed_value = *(unsigned long*)value;
    g_seed_span = (char*)value_end - (char*)value;
    fields[0] = 0x11111111;
    fields[1] = 0x22222222;
    fields[2] = 0x33333333;
}

extern "C" void __fastcall
CManager_CManager_0066eab7_ConstructScene(void* scene, void*)
{
    g_scene_sequence = ++g_sequence;
    g_scene_this = scene;
    *(unsigned char*)scene = 0x5A;
}

extern "C" void __fastcall
CManager_CManager_0066eab7_InitialiseScrollIndices(
    void* manager, void*, long count)
{
    Manager* self = (Manager*)manager;
    int i;
    g_indices_sequence = ++g_sequence;
    g_indices_manager = manager;
    g_indices_count = count;
    g_indices_saw_zero_state = self->open == 0;
    for (i = 0; i < 9; ++i)
    {
        if (self->state[i] != 0)
            g_indices_saw_zero_state = 0;
    }
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
    void* world = (void*)0x44444444;
    void* result;
    int i;

    memset(&manager, 0xCC, sizeof(manager));
    g_sequence = 0;
    g_screens_sequence = 0;
    g_scene_sequence = 0;
    g_indices_sequence = 0;
    g_screens_this = 0;
    g_screens_count = 0;
    g_seed_value = 1;
    g_seed_span = 0;
    g_scene_this = 0;
    g_indices_manager = 0;
    g_indices_count = 0;
    g_indices_saw_zero_state = 0;
    result = CManager_CManager_0066eab7(&manager, 0, world);

    ok &= Check(result == &manager,
                "constructor must return this");
    ok &= Check(manager.world == world,
                "constructor must retain the world pointer");
    ok &= Check(g_screens_this == manager.screens &&
                g_screens_count == 11,
                "embedded screen container must receive this+4 and count 11");
    ok &= Check(g_seed_value == 0 && g_seed_span == 3,
                "screen container must receive the zero stack seed range");
    ok &= Check(manager.screens[0] == 0x11111111 &&
                manager.screens[1] == 0x22222222 &&
                manager.screens[2] == 0x33333333,
                "screen-container constructor results must be preserved");
    for (i = 0; i < 9; ++i)
    {
        if (manager.state[i] != 0)
            ok &= Check(0, "navigation and vector state must be zeroed");
    }
    ok &= Check(manager.open == 0,
                "open flag must initialize false");
    ok &= Check(g_scene_this == manager.scene &&
                manager.scene[0] == 0x5A,
                "embedded scene constructor must receive this+0x38");
    ok &= Check(g_indices_manager == &manager &&
                g_indices_count == 5,
                "five scroll indices must be initialized");
    ok &= Check(g_indices_saw_zero_state,
                "state must be cleared before scroll-index initialization");
    ok &= Check(g_screens_sequence == 1 &&
                g_scene_sequence == 2 &&
                g_indices_sequence == 3,
                "embedded construction order must match retail");

    if (!ok)
        return 1;

    puts("PASS CManager::CManager");
    return 0;
}
