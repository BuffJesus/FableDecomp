#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
Vector_InsertValue_0066eda6(void* vector, void*, const void* value);

struct PointerVector
{
    void** begin;
    void** end;
    void** capacity;
};

static int g_reallocate_count;
static void* g_reallocate_vector;
static void* g_position;
static void* g_value_source;
static void* g_value_end;
static long g_count;
static long g_grow;

extern "C" void __fastcall
Vector_InsertValue_0066eda6_Reallocate(
    void* vector, void*, void* position, void* value, void* value_end,
    long count, long grow)
{
    PointerVector* values = (PointerVector*)vector;
    ++g_reallocate_count;
    g_reallocate_vector = vector;
    g_position = position;
    g_value_source = value;
    g_value_end = value_end;
    g_count = count;
    g_grow = grow;
    *(void**)position = *(void**)value;
    values->end = (void**)position + 1;
    values->capacity = values->end + 2;
}

static void ResetCalls()
{
    g_reallocate_count = 0;
    g_reallocate_vector = 0;
    g_position = 0;
    g_value_source = 0;
    g_value_end = 0;
    g_count = 0;
    g_grow = 0;
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
    PointerVector vector;
    void* storage[6];
    void* value;

    memset(storage, 0, sizeof(storage));
    storage[0] = (void*)0x11111111;
    vector.begin = storage;
    vector.end = storage + 1;
    vector.capacity = storage + 4;
    value = (void*)0x22222222;
    ResetCalls();
    Vector_InsertValue_0066eda6(&vector, 0, &value);
    ok &= Check(g_reallocate_count == 0,
                "spare capacity must use the direct insertion path");
    ok &= Check(storage[1] == value,
                "direct insertion must copy the referenced value");
    ok &= Check(vector.end == storage + 2 &&
                vector.capacity == storage + 4,
                "direct insertion must advance end without changing capacity");

    storage[0] = (void*)0x33333333;
    storage[1] = (void*)0x44444444;
    vector.begin = storage;
    vector.end = storage + 2;
    vector.capacity = storage + 2;
    value = (void*)0x55555555;
    ResetCalls();
    Vector_InsertValue_0066eda6(&vector, 0, &value);
    ok &= Check(g_reallocate_count == 1 &&
                g_reallocate_vector == &vector,
                "full vector must delegate to reallocation");
    ok &= Check(g_position == storage + 2 &&
                g_value_source == &value &&
                g_value_end != 0,
                "reallocator must receive end position and source range");
    ok &= Check(g_count == 1 && g_grow == 1,
                "reallocator must receive single-element growth values");
    ok &= Check(storage[2] == value &&
                vector.end == storage + 3,
                "reallocation path must insert the value at old end");

    if (!ok)
        return 1;

    puts("PASS Vector_InsertValue 0x0066EDA6");
    return 0;
}
