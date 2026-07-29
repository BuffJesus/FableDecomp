#include <cstdio>

#include "../../../src/compiled/00/99/CDataInputStream_CDataInputStream_00993b20.cpp"

struct TestDataInputStream_00993b20
{
    void* vtable;
    unsigned long fields[5];
};

extern "C" void* CDataInputStream_VTable_00993b20[1] = {
    (void*)0x12345678
};

static volatile int g_BaseCalls;
static void* volatile g_BaseThis;
static volatile int g_BaseSawOriginalValues;

extern "C" void* __fastcall
CDataInputStream_ConstructBase_00993b20(void* stream, void*)
{
    TestDataInputStream_00993b20* value =
        (TestDataInputStream_00993b20*)stream;
    ++g_BaseCalls;
    g_BaseThis = stream;
    g_BaseSawOriginalValues =
        value->vtable == (void*)0x11111111 &&
        value->fields[0] == 0x22222222 &&
        value->fields[1] == 0x33333333 &&
        value->fields[2] == 0x44444444 &&
        value->fields[3] == 0x55555555 &&
        value->fields[4] == 0x66666666;

    value->vtable = (void*)0xaaaaaaaa;
    for (int index = 0; index != 5; ++index)
        value->fields[index] = 0xbbbbbbbb;
    return stream;
}

int main()
{
    TestDataInputStream_00993b20 stream;
    stream.vtable = (void*)0x11111111;
    stream.fields[0] = 0x22222222;
    stream.fields[1] = 0x33333333;
    stream.fields[2] = 0x44444444;
    stream.fields[3] = 0x55555555;
    stream.fields[4] = 0x66666666;
    g_BaseCalls = 0;
    g_BaseThis = 0;
    g_BaseSawOriginalValues = 0;

    typedef void* (__fastcall *ConstructorFunction)(void*, void*);
    ConstructorFunction volatile construct =
        &CDataInputStream_CDataInputStream_00993b20;
    void* result = construct(&stream, 0);

    int failures = 0;
    if (result != &stream ||
        g_BaseCalls != 1 ||
        g_BaseThis != &stream ||
        g_BaseSawOriginalValues != 1 ||
        stream.vtable != CDataInputStream_VTable_00993b20)
    {
        ++failures;
    }

    for (int index = 0; index != 5; ++index)
    {
        if (stream.fields[index] != 0)
            ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "DATA_INPUT_STREAM_00993b20_TEST FAIL count=%d base_calls=%d\n",
            failures,
            g_BaseCalls);
        return 1;
    }

    std::printf("DATA_INPUT_STREAM_00993b20_TEST PASS\n");
    return 0;
}
