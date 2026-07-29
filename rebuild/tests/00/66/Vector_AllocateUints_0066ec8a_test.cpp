#include <stdio.h>
#include <string.h>

extern "C" void* __fastcall
Vector_AllocateUints_0066ec8a(
    void* vector, void*, unsigned long count, void* unused);

struct UIntVector
{
    unsigned long* begin;
    unsigned long* end;
    unsigned long* capacity;
};

static unsigned long g_storage[16];
static int g_allocate_count;
static unsigned long g_allocate_bytes;

extern "C" void* __cdecl
Vector_AllocateUints_0066ec8a_Allocate(unsigned long bytes)
{
    ++g_allocate_count;
    g_allocate_bytes = bytes;
    return g_storage;
}

static void ResetCalls()
{
    g_allocate_count = 0;
    g_allocate_bytes = 0;
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
    UIntVector vector;
    void* result;

    memset(&vector, 0xCC, sizeof(vector));
    ResetCalls();
    result = Vector_AllocateUints_0066ec8a(
        &vector, 0, 0, (void*)0x11111111);
    ok &= Check(result == &vector,
                "zero-count allocation must return the vector");
    ok &= Check(vector.begin == 0 &&
                vector.end == 0 &&
                vector.capacity == 0,
                "zero-count allocation must clear all vector fields");
    ok &= Check(g_allocate_count == 0,
                "zero count must not call the allocator");

    memset(&vector, 0xCC, sizeof(vector));
    ResetCalls();
    result = Vector_AllocateUints_0066ec8a(
        &vector, 0, 5, (void*)0x22222222);
    ok &= Check(result == &vector,
                "populated allocation must return the vector");
    ok &= Check(g_allocate_count == 1 && g_allocate_bytes == 20,
                "allocator must receive count multiplied by four");
    ok &= Check(vector.begin == g_storage &&
                vector.end == g_storage,
                "new vector must begin empty at the allocation");
    ok &= Check(vector.capacity == g_storage + 5,
                "capacity must end after count unsigned longs");

    if (!ok)
        return 1;

    puts("PASS Vector_AllocateUints");
    return 0;
}
