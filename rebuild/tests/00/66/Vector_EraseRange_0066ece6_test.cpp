#include <stdio.h>
#include <string.h>

extern "C" void* __fastcall
Vector_EraseRange_0066ece6(
    void* vector, void*, void* erase_begin, void* erase_end);

struct PointerVector
{
    void** begin;
    void** end;
    void** capacity;
};

static int g_move_count;
static void* g_source_begin;
static void* g_source_end;
static void* g_destination;

extern "C" void* __fastcall
Vector_EraseRange_0066ece6_Move(
    void* source_begin, void* source_end, void* destination)
{
    void** source = (void**)source_begin;
    void** end = (void**)source_end;
    void** output = (void**)destination;
    ++g_move_count;
    g_source_begin = source_begin;
    g_source_end = source_end;
    g_destination = destination;
    while (source != end)
    {
        *output++ = *source++;
    }
    return output;
}

static void ResetCalls()
{
    g_move_count = 0;
    g_source_begin = 0;
    g_source_end = 0;
    g_destination = 0;
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
    void* result;

    storage[0] = (void*)0x11111111;
    storage[1] = (void*)0x22222222;
    storage[2] = (void*)0x33333333;
    storage[3] = (void*)0x44444444;
    storage[4] = (void*)0x55555555;
    storage[5] = (void*)0x66666666;
    vector.begin = storage;
    vector.end = storage + 5;
    vector.capacity = storage + 6;
    ResetCalls();
    result = Vector_EraseRange_0066ece6(
        &vector, 0, storage + 1, storage + 3);
    ok &= Check(result == storage + 1,
                "partial erase must return erase_begin");
    ok &= Check(g_move_count == 1 &&
                g_source_begin == storage + 3 &&
                g_source_end == storage + 5 &&
                g_destination == storage + 1,
                "partial erase must move the exact trailing range");
    ok &= Check(vector.end == storage + 3,
                "partial erase must store the moved destination end");
    ok &= Check(storage[0] == (void*)0x11111111 &&
                storage[1] == (void*)0x44444444 &&
                storage[2] == (void*)0x55555555,
                "partial erase must compact trailing values");

    vector.end = storage + 3;
    ResetCalls();
    result = Vector_EraseRange_0066ece6(
        &vector, 0, storage + 2, storage + 2);
    ok &= Check(result == storage + 2,
                "empty erase must return its position");
    ok &= Check(g_source_begin == storage + 2 &&
                g_source_end == storage + 3 &&
                g_destination == storage + 2,
                "empty erase must still route the unchanged tail");
    ok &= Check(vector.end == storage + 3,
                "empty erase must preserve vector end");

    ResetCalls();
    result = Vector_EraseRange_0066ece6(
        &vector, 0, storage, storage + 3);
    ok &= Check(result == storage,
                "full erase must return vector begin");
    ok &= Check(g_source_begin == storage + 3 &&
                g_source_end == storage + 3 &&
                g_destination == storage,
                "full erase must move an empty tail to begin");
    ok &= Check(vector.end == storage,
                "full erase must leave the vector empty");

    if (!ok)
        return 1;

    puts("PASS Vector_EraseRange 0x0066ECE6");
    return 0;
}
