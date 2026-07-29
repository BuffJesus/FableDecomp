#include <stdio.h>
#include <string.h>

extern "C" void* __fastcall
Vector_ConstructFill_0066ecbe(
    void* vector, void*, unsigned long count,
    const unsigned long* value, void* allocator);

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

extern "C" void* __fastcall
Vector_AllocateUints_0066ec8a(
    void* raw_vector, void*, unsigned long count, void*)
{
    UIntVector* vector = static_cast<UIntVector*>(raw_vector);
    vector->begin = count == 0
        ? 0
        : static_cast<unsigned long*>(
            Vector_AllocateUints_0066ec8a_Allocate(count * 4));
    vector->end = vector->begin;
    vector->capacity = vector->begin + count;
    return vector;
}

extern "C" unsigned long* __fastcall
std_Fill_n_ulong_0066ec31(
    unsigned long* destination,
    unsigned long count,
    const unsigned long* value)
{
    while (count-- != 0)
        *destination++ = *value;
    return destination;
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
    unsigned long value;
    void* result;

    memset(&vector, 0xCC, sizeof(vector));
    value = 0x11111111;
    ResetCalls();
    result = Vector_ConstructFill_0066ecbe(
        &vector, 0, 0, &value, (void*)0x22222222);
    ok &= Check(result == &vector,
                "zero-count construction must return the vector");
    ok &= Check(vector.begin == 0 &&
                vector.end == 0 &&
                vector.capacity == 0,
                "zero-count construction must produce an empty null vector");
    ok &= Check(g_allocate_count == 0,
                "zero-count construction must not allocate");

    memset(&vector, 0xCC, sizeof(vector));
    memset(g_storage, 0xCC, sizeof(g_storage));
    value = 0xA5A5A5A5;
    ResetCalls();
    result = Vector_ConstructFill_0066ecbe(
        &vector, 0, 4, &value, (void*)0x33333333);
    ok &= Check(result == &vector,
                "filled construction must return the vector");
    ok &= Check(g_allocate_count == 1 && g_allocate_bytes == 16,
                "filled construction must allocate count times four bytes");
    ok &= Check(vector.begin == g_storage &&
                vector.end == g_storage + 4 &&
                vector.capacity == g_storage + 4,
                "filled vector bounds must span all constructed elements");
    ok &= Check(g_storage[0] == value &&
                g_storage[1] == value &&
                g_storage[2] == value &&
                g_storage[3] == value,
                "every constructed element must copy the referenced value");

    if (!ok)
        return 1;

    puts("PASS Vector_ConstructFill 0x0066ECBE");
    return 0;
}
