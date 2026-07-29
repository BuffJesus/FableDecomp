#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
CManager_GetAllRegisteredScreens_0066e941(void* manager, void*, void* output);

struct PointerVector
{
    void** begin;
    void** end;
    void** capacity;
};

struct Manager
{
    unsigned long prefix;
    void** screens_begin;
    void** screens_end;
    void** screens_capacity;
};

static int g_clear_count;
static int g_insert_count;
static void* g_clear_begin;
static void* g_clear_end;
static void* g_insert_sources[8];

static void ResetCalls()
{
    g_clear_count = 0;
    g_insert_count = 0;
    g_clear_begin = 0;
    g_clear_end = 0;
    memset(g_insert_sources, 0, sizeof(g_insert_sources));
}

extern "C" void __fastcall
CManager_GetAllRegisteredScreens_0066e941_Clear(
    void* output, void*, void* begin, void* end)
{
    PointerVector* vector = (PointerVector*)output;
    ++g_clear_count;
    g_clear_begin = begin;
    g_clear_end = end;
    vector->end = vector->begin;
}

extern "C" void __fastcall
CManager_GetAllRegisteredScreens_0066e941_Insert(
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
    void* screens[5];
    void* output_storage[6];

    screens[0] = (void*)0x11111111;
    screens[1] = 0;
    screens[2] = (void*)0x22222222;
    screens[3] = 0;
    screens[4] = (void*)0x33333333;
    manager.prefix = 0;
    manager.screens_begin = screens;
    manager.screens_end = screens + 5;
    manager.screens_capacity = screens + 5;

    output_storage[0] = (void*)0xAAAAAAAA;
    output_storage[1] = (void*)0xBBBBBBBB;
    output.begin = output_storage;
    output.end = output_storage + 2;
    output.capacity = output_storage + 6;
    ResetCalls();
    CManager_GetAllRegisteredScreens_0066e941(&manager, 0, &output);
    ok &= Check(g_clear_count == 1 &&
                g_clear_begin == output_storage &&
                g_clear_end == output_storage + 2,
                "existing output range must be cleared first");
    ok &= Check(g_insert_count == 3,
                "only non-null registered screens must be inserted");
    ok &= Check(g_insert_sources[0] == &screens[0] &&
                g_insert_sources[1] == &screens[2] &&
                g_insert_sources[2] == &screens[4],
                "insert helper must receive source element addresses in order");
    ok &= Check(output.end == output.begin + 3 &&
                output.begin[0] == screens[0] &&
                output.begin[1] == screens[2] &&
                output.begin[2] == screens[4],
                "output must contain compacted non-null screens");

    manager.screens_end = manager.screens_begin;
    output.end = output.begin + 3;
    ResetCalls();
    CManager_GetAllRegisteredScreens_0066e941(&manager, 0, &output);
    ok &= Check(g_clear_count == 1 && g_insert_count == 0,
                "empty manager must clear output without inserting");
    ok &= Check(output.end == output.begin,
                "empty manager must leave an empty output vector");

    if (!ok)
        return 1;

    puts("PASS CManager::GetAllRegisteredScreens");
    return 0;
}
