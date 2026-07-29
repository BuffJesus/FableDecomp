#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/48/ByteVector_InsertFill_00485f3a.cpp"

struct TestByteVector_00485f3a
{
    unsigned char* begin;
    unsigned char* end;
    unsigned char* capacityEnd;
};

static int g_MoveConstructCalls;
static int g_MoveBackwardCalls;
static int g_FillCalls;
static int g_ReallocateCalls;
static void* g_VectorSeen;
static unsigned char* g_PositionSeen;
static const unsigned char* g_ValueSeen;
static void* g_AuxSeen;
static unsigned long g_CountSeen;
static int g_ZeroSeen;

extern "C" void __fastcall
ByteVector_MoveConstructTail_00485f3a(
    void* beginValue,
    void* endValue,
    void* destinationEndValue)
{
    ++g_MoveConstructCalls;
    unsigned char* begin = (unsigned char*)beginValue;
    unsigned char* end = (unsigned char*)endValue;
    unsigned char* destinationEnd = (unsigned char*)destinationEndValue;
    while (end != begin)
    {
        --end;
        --destinationEnd;
        *destinationEnd = *end;
    }
}

extern "C" void __fastcall
ByteVector_MoveTailBackward_00485f3a(
    void* beginValue,
    void* endValue,
    void* destinationEndValue)
{
    ++g_MoveBackwardCalls;
    unsigned char* begin = (unsigned char*)beginValue;
    unsigned char* end = (unsigned char*)endValue;
    unsigned char* destinationEnd = (unsigned char*)destinationEndValue;
    while (end != begin)
    {
        --end;
        --destinationEnd;
        *destinationEnd = *end;
    }
}

extern "C" void __fastcall
ByteVector_FillRange_00485f3a(
    void* beginValue,
    void* endValue,
    const unsigned char* value)
{
    ++g_FillCalls;
    unsigned char* begin = (unsigned char*)beginValue;
    unsigned char* end = (unsigned char*)endValue;
    while (begin != end)
    {
        *begin++ = *value;
    }
}

extern "C" void __fastcall
ByteVector_ReallocateAndInsert_00485f3a(
    void* vector,
    void*,
    unsigned char* position,
    const unsigned char* value,
    void* auxiliary,
    unsigned long count,
    int zero)
{
    ++g_ReallocateCalls;
    g_VectorSeen = vector;
    g_PositionSeen = position;
    g_ValueSeen = value;
    g_AuxSeen = auxiliary;
    g_CountSeen = count;
    g_ZeroSeen = zero;
}

static void ResetProbes()
{
    g_MoveConstructCalls = 0;
    g_MoveBackwardCalls = 0;
    g_FillCalls = 0;
    g_ReallocateCalls = 0;
    g_VectorSeen = 0;
    g_PositionSeen = 0;
    g_ValueSeen = 0;
    g_AuxSeen = 0;
    g_CountSeen = 0;
    g_ZeroSeen = -1;
}

int main()
{
    typedef void (__fastcall *InsertFunction)(
        void*,
        void*,
        unsigned char*,
        unsigned long,
        const unsigned char*);
    InsertFunction volatile insert = &ByteVector_InsertFill_00485f3a;

    int failures = 0;
    unsigned char fillValue = 0;
    TestByteVector_00485f3a empty;
    empty.begin = 0;
    empty.end = 0;
    empty.capacityEnd = 0;

    ResetProbes();
    insert(&empty, 0, empty.end, 12, &fillValue);
    if (g_ReallocateCalls != 1 ||
        g_VectorSeen != &empty ||
        g_PositionSeen != empty.end ||
        g_ValueSeen != &fillValue ||
        g_AuxSeen == 0 ||
        g_CountSeen != 12 ||
        g_ZeroSeen != 0 ||
        g_MoveConstructCalls != 0 ||
        g_MoveBackwardCalls != 0 ||
        g_FillCalls != 0)
    {
        ++failures;
    }

    unsigned char storage[12];
    std::memset(storage, 0xCC, sizeof(storage));
    storage[0] = 0x11;
    storage[1] = 0x22;
    TestByteVector_00485f3a reserved;
    reserved.begin = storage;
    reserved.end = storage + 2;
    reserved.capacityEnd = storage + sizeof(storage);
    fillValue = 0x7A;

    ResetProbes();
    insert(&reserved, 0, reserved.end, 3, &fillValue);
    if (g_ReallocateCalls != 0 ||
        g_MoveConstructCalls != 1 ||
        g_MoveBackwardCalls != 0 ||
        g_FillCalls != 2 ||
        reserved.end != storage + 5 ||
        storage[0] != 0x11 ||
        storage[1] != 0x22 ||
        storage[2] != 0x7A ||
        storage[3] != 0x7A ||
        storage[4] != 0x7A)
    {
        ++failures;
    }

    ResetProbes();
    insert(&reserved, 0, reserved.end, 0, &fillValue);
    if (g_ReallocateCalls != 0 ||
        g_MoveConstructCalls != 0 ||
        g_MoveBackwardCalls != 0 ||
        g_FillCalls != 0 ||
        reserved.end != storage + 5)
    {
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "BYTE_VECTOR_INSERT_00485f3a_TEST FAIL count=%d realloc=%d fill=%d move=%d\n",
            failures,
            g_ReallocateCalls,
            g_FillCalls,
            g_MoveConstructCalls);
        return 1;
    }

    std::printf("BYTE_VECTOR_INSERT_00485f3a_TEST PASS\n");
    return 0;
}
