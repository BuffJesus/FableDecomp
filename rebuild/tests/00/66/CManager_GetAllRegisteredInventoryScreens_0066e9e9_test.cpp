#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
CManager_GetAllRegisteredInventoryScreens_0066e9e9(
    void* manager, void*, void* output);

struct PointerVector
{
    void** begin;
    void** end;
    void** capacity;
};

struct Screen;
typedef unsigned char (__fastcall* IsInventoryFunction)(Screen*, void*);

struct ScreenVtable
{
    void* slots[3];
    IsInventoryFunction is_inventory;
};

struct Screen
{
    ScreenVtable* vtable;
    unsigned char inventory;
    int identifier;
};

struct Manager
{
    unsigned long prefix;
    Screen** screens_begin;
    Screen** screens_end;
    Screen** screens_capacity;
};

static int g_clear_count;
static int g_insert_count;
static int g_predicate_count;
static int g_predicate_order[8];
static void* g_clear_begin;
static void* g_clear_end;
static void* g_insert_sources[8];

static unsigned char __fastcall IsInventory(Screen* screen, void*)
{
    g_predicate_order[g_predicate_count++] = screen->identifier;
    return screen->inventory;
}

static void ResetCalls()
{
    g_clear_count = 0;
    g_insert_count = 0;
    g_predicate_count = 0;
    memset(g_predicate_order, 0, sizeof(g_predicate_order));
    g_clear_begin = 0;
    g_clear_end = 0;
    memset(g_insert_sources, 0, sizeof(g_insert_sources));
}

extern "C" void __fastcall
CManager_GetAllRegisteredInventoryScreens_0066e9e9_Clear(
    void* output, void*, void* begin, void* end)
{
    PointerVector* vector = (PointerVector*)output;
    ++g_clear_count;
    g_clear_begin = begin;
    g_clear_end = end;
    vector->end = vector->begin;
}

extern "C" void __fastcall
CManager_GetAllRegisteredInventoryScreens_0066e9e9_Insert(
    void* output, void*, void* value)
{
    PointerVector* vector = (PointerVector*)output;
    g_insert_sources[g_insert_count++] = value;
    if (vector->end != vector->capacity)
    {
        *vector->end = *(void**)value;
        ++vector->end;
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
    PointerVector output;
    ScreenVtable vtable;
    Screen screen0;
    Screen screen1;
    Screen screen2;
    Screen screen3;
    Screen* screens[6];
    void* output_storage[6];

    memset(&vtable, 0, sizeof(vtable));
    vtable.is_inventory = IsInventory;
    screen0.vtable = &vtable;
    screen0.inventory = 1;
    screen0.identifier = 0;
    screen1.vtable = &vtable;
    screen1.inventory = 0;
    screen1.identifier = 1;
    screen2.vtable = &vtable;
    screen2.inventory = 1;
    screen2.identifier = 2;
    screen3.vtable = &vtable;
    screen3.inventory = 0;
    screen3.identifier = 3;
    screens[0] = &screen0;
    screens[1] = 0;
    screens[2] = &screen1;
    screens[3] = &screen2;
    screens[4] = &screen3;
    screens[5] = 0;
    manager.prefix = 0;
    manager.screens_begin = screens;
    manager.screens_end = screens + 6;
    manager.screens_capacity = screens + 6;

    output_storage[0] = (void*)0xAAAAAAAA;
    output_storage[1] = (void*)0xBBBBBBBB;
    output.begin = output_storage;
    output.end = output_storage + 2;
    output.capacity = output_storage + 6;
    ResetCalls();
    CManager_GetAllRegisteredInventoryScreens_0066e9e9(
        &manager, 0, &output);
    ok &= Check(g_clear_count == 1 &&
                g_clear_begin == output_storage &&
                g_clear_end == output_storage + 2,
                "existing output range must be cleared first");
    ok &= Check(g_predicate_count == 4 &&
                g_predicate_order[0] == 0 &&
                g_predicate_order[1] == 1 &&
                g_predicate_order[2] == 2 &&
                g_predicate_order[3] == 3,
                "predicate must run for each non-null screen in order");
    ok &= Check(g_insert_count == 2 &&
                g_insert_sources[0] == &screens[0] &&
                g_insert_sources[1] == &screens[3],
                "only predicate-approved source elements must be inserted");
    ok &= Check(output.end == output.begin + 2 &&
                output.begin[0] == &screen0 &&
                output.begin[1] == &screen2,
                "output must contain inventory screens in registration order");

    manager.screens_end = manager.screens_begin;
    output.end = output.begin + 2;
    ResetCalls();
    CManager_GetAllRegisteredInventoryScreens_0066e9e9(
        &manager, 0, &output);
    ok &= Check(g_clear_count == 1 &&
                g_predicate_count == 0 &&
                g_insert_count == 0,
                "empty manager must only clear output");
    ok &= Check(output.end == output.begin,
                "empty manager must leave empty output");

    if (!ok)
        return 1;

    puts("PASS CManager::GetAllRegisteredInventoryScreens");
    return 0;
}
