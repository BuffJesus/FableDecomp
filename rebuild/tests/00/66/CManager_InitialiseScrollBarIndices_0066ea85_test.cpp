#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
CManager_InitialiseScrollBarIndices_0066ea85(
    void* manager, void*, long count);

struct LongVector
{
    long* begin;
    long* end;
    long* capacity;
};

struct Manager
{
    unsigned char prefix[0x1C];
    LongVector scroll_indices;
};

static long g_storage[8];
static int g_clear_count;
static int g_resize_count;
static void* g_clear_begin;
static void* g_clear_end;
static long g_resize_value;

static void ResetCalls()
{
    g_clear_count = 0;
    g_resize_count = 0;
    g_clear_begin = 0;
    g_clear_end = 0;
    g_resize_value = -1;
}

extern "C" void __fastcall
CManager_InitialiseScrollBarIndices_0066ea85_Clear(
    void* vector, void*, void* begin, void* end)
{
    LongVector* values = (LongVector*)vector;
    ++g_clear_count;
    g_clear_begin = begin;
    g_clear_end = end;
    values->end = values->begin;
}

extern "C" void __fastcall
CManager_InitialiseScrollBarIndices_0066ea85_Resize(
    void* vector, void*, long count)
{
    LongVector* values = (LongVector*)vector;
    ++g_resize_count;
    g_resize_value = count;
    values->begin = g_storage;
    values->end = g_storage + count;
    values->capacity = g_storage + 8;
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
    long old_storage[3];

    old_storage[0] = 9;
    old_storage[1] = 8;
    old_storage[2] = 7;
    memset(&manager, 0, sizeof(manager));
    manager.scroll_indices.begin = old_storage;
    manager.scroll_indices.end = old_storage + 3;
    manager.scroll_indices.capacity = old_storage + 3;
    memset(g_storage, 0xCC, sizeof(g_storage));
    ResetCalls();
    CManager_InitialiseScrollBarIndices_0066ea85(&manager, 0, 5);
    ok &= Check(g_clear_count == 1 &&
                g_clear_begin == old_storage &&
                g_clear_end == old_storage + 3,
                "existing scroll-index range must be cleared first");
    ok &= Check(g_resize_count == 1 && g_resize_value == 5,
                "scroll-index vector must be resized to requested count");
    ok &= Check(manager.scroll_indices.begin == g_storage &&
                manager.scroll_indices.end == g_storage + 5,
                "resized vector bounds must be retained");
    ok &= Check(g_storage[0] == 0 &&
                g_storage[1] == 1 &&
                g_storage[2] == 2 &&
                g_storage[3] == 3 &&
                g_storage[4] == 4,
                "resized vector must be filled with sequential indices");

    memset(g_storage, 0xCC, sizeof(g_storage));
    ResetCalls();
    CManager_InitialiseScrollBarIndices_0066ea85(&manager, 0, 2);
    ok &= Check(g_clear_count == 1 &&
                g_clear_begin == g_storage &&
                g_clear_end == g_storage + 5,
                "reinitialization must clear the prior active range");
    ok &= Check(g_resize_count == 1 && g_resize_value == 2,
                "reinitialization must apply the new count");
    ok &= Check(g_storage[0] == 0 && g_storage[1] == 1,
                "reinitialization must rebuild sequential indices");

    g_storage[0] = 123;
    manager.scroll_indices.begin = g_storage;
    manager.scroll_indices.end = g_storage + 2;
    ResetCalls();
    CManager_InitialiseScrollBarIndices_0066ea85(&manager, 0, 0);
    ok &= Check(g_clear_count == 1 &&
                g_resize_count == 1 &&
                g_resize_value == 0,
                "zero count must still clear and resize");
    ok &= Check(manager.scroll_indices.end == manager.scroll_indices.begin,
                "zero count must produce an empty vector");
    ok &= Check(g_storage[0] == 123,
                "zero count must skip the fill loop");

    if (!ok)
        return 1;

    puts("PASS CManager::InitialiseScrollBarIndices");
    return 0;
}
