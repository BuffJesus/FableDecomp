#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CManager_Open_0066ea46(void* manager, void*);

struct Manager
{
    unsigned char prefix[0x10];
    unsigned char inventory_vector[0x0C];
    unsigned char padding1c[0x18];
    unsigned char open;
    unsigned char padding35[3];
    unsigned char scene[0x20];
};

static int g_scene_count;
static int g_inventory_count;
static int g_sequence;
static int g_scene_sequence;
static int g_inventory_sequence;
static void* g_scene_this;
static void* g_inventory_manager;
static void* g_inventory_output;
static unsigned long g_one;
static unsigned long g_zero;
static unsigned long g_colour;
static unsigned char g_open_seen_by_inventory;

static void ResetCalls()
{
    g_scene_count = 0;
    g_inventory_count = 0;
    g_sequence = 0;
    g_scene_sequence = 0;
    g_inventory_sequence = 0;
    g_scene_this = 0;
    g_inventory_manager = 0;
    g_inventory_output = 0;
    g_one = 0;
    g_zero = 0;
    g_colour = 0;
    g_open_seen_by_inventory = 0;
}

extern "C" void __fastcall
CManager_Open_0066ea46_StartScene(
    void* scene, void*, unsigned long one, unsigned long zero, unsigned long colour)
{
    ++g_scene_count;
    g_scene_sequence = ++g_sequence;
    g_scene_this = scene;
    g_one = one;
    g_zero = zero;
    g_colour = colour;
}

extern "C" void __fastcall
CManager_Open_0066ea46_GetInventoryScreens(
    void* manager, void*, void* output)
{
    ++g_inventory_count;
    g_inventory_sequence = ++g_sequence;
    g_inventory_manager = manager;
    g_inventory_output = output;
    g_open_seen_by_inventory = ((Manager*)manager)->open;
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

    memset(&manager, 0xCC, sizeof(manager));
    manager.open = 0;
    ResetCalls();
    CManager_Open_0066ea46(&manager, 0);
    ok &= Check(g_scene_count == 1 && g_scene_this == manager.scene,
                "first open must start the embedded scene");
    ok &= Check(g_one == 0x3F800000 &&
                g_zero == 0x00000000 &&
                g_colour == 0xFF000000,
                "scene startup payload must match retail constants");
    ok &= Check(manager.open == 1,
                "first open must set the open flag");
    ok &= Check(g_inventory_count == 1 &&
                g_inventory_manager == &manager &&
                g_inventory_output == manager.inventory_vector,
                "first open must refresh embedded inventory output");
    ok &= Check(g_open_seen_by_inventory == 1,
                "open flag must be set before inventory refresh");
    ok &= Check(g_scene_sequence == 1 && g_inventory_sequence == 2,
                "scene startup must precede inventory refresh");

    manager.open = 1;
    ResetCalls();
    CManager_Open_0066ea46(&manager, 0);
    ok &= Check(g_scene_count == 0,
                "repeated open must not restart the scene");
    ok &= Check(manager.open == 1,
                "repeated open must retain the open flag");
    ok &= Check(g_inventory_count == 1 &&
                g_inventory_manager == &manager &&
                g_inventory_output == manager.inventory_vector,
                "repeated open must still refresh inventory screens");
    ok &= Check(g_open_seen_by_inventory == 1 &&
                g_inventory_sequence == 1,
                "repeated refresh must observe the open state");

    if (!ok)
        return 1;

    puts("PASS CManager::Open");
    return 0;
}
