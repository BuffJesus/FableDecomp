#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/48/CPersistContextReadSection_CPersistContextReadSection_00485cd2.cpp"

struct TestPersistContextReadSection_00485cd2
{
    unsigned char bytes[0x74];
};

static int g_Sequence;
static int g_Field2CSequence;
static int g_Field48Sequence;
static int g_NameSequence;
static void* g_Field2CSeen;
static void* g_Field48Seen;
static void* g_NameSeen;

extern "C" void __fastcall
CPersistContextReadSection_ConstructField2C_00485cd2(
    void* value,
    void*)
{
    g_Field2CSequence = ++g_Sequence;
    g_Field2CSeen = value;
    *(unsigned char*)value = 0x2C;
}

extern "C" void __fastcall
CPersistContextReadSection_ConstructField48_00485cd2(
    void* value,
    void*)
{
    g_Field48Sequence = ++g_Sequence;
    g_Field48Seen = value;
    *(unsigned char*)value = 0x48;
}

extern "C" void __fastcall
CPersistContextReadSection_ConstructName_00485cd2(
    void* value,
    void*)
{
    g_NameSequence = ++g_Sequence;
    g_NameSeen = value;
    *(unsigned char*)value = 0x70;
}

int main()
{
    TestPersistContextReadSection_00485cd2 section;
    std::memset(&section, 0xA5, sizeof(section));
    g_Sequence = 0;
    g_Field2CSequence = 0;
    g_Field48Sequence = 0;
    g_NameSequence = 0;
    g_Field2CSeen = 0;
    g_Field48Seen = 0;
    g_NameSeen = 0;

    typedef void* (__fastcall *ConstructFunction)(void*, void*);
    ConstructFunction volatile construct =
        &CPersistContextReadSection_CPersistContextReadSection_00485cd2;
    void* result = construct(&section, 0);

    int failures = 0;
    if (result != &section ||
        g_Sequence != 3 ||
        g_Field2CSequence != 1 ||
        g_Field48Sequence != 2 ||
        g_NameSequence != 3 ||
        g_Field2CSeen != section.bytes + 0x2C ||
        g_Field48Seen != section.bytes + 0x48 ||
        g_NameSeen != section.bytes + 0x70 ||
        section.bytes[0x2C] != 0x2C ||
        section.bytes[0x48] != 0x48 ||
        section.bytes[0x70] != 0x70 ||
        section.bytes[0] != 0xA5 ||
        section.bytes[0x73] != 0xA5)
    {
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "PERSIST_READ_SECTION_CTOR_00485cd2_TEST FAIL count=%d sequence=%d\n",
            failures,
            g_Sequence);
        return 1;
    }

    std::printf("PERSIST_READ_SECTION_CTOR_00485cd2_TEST PASS\n");
    return 0;
}
