#include <stdio.h>
#include <string.h>

extern "C" void* __fastcall
CCharString_ConstructCCoopSpiritDef_0066fa10(
    void* string, void*);

extern "C" char CCharString_ConstructCCoopSpiritDef_0066fa10_Name[] =
    "CCoopSpiritDef";

struct StringStorage
{
    unsigned long marker;
};

static int g_construct_calls;
static void* g_construct_string;
static const char* g_construct_text;
static long g_construct_length;

extern "C" void* __fastcall
CCharString_ConstructCCoopSpiritDef_0066fa10_Construct(
    void* string, void*, const char* text, long length)
{
    ++g_construct_calls;
    g_construct_string = string;
    g_construct_text = text;
    g_construct_length = length;
    ((StringStorage*)string)->marker = 0x12345678;
    return 0;
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
    StringStorage string;
    void* returned;

    string.marker = 0;
    g_construct_calls = 0;
    g_construct_string = 0;
    g_construct_text = 0;
    g_construct_length = 0;

    returned = CCharString_ConstructCCoopSpiritDef_0066fa10(&string, 0);
    ok &= Check(returned == &string,
                "wrapper must return its destination");
    ok &= Check(g_construct_calls == 1 &&
                g_construct_string == &string,
                "CCharString constructor must receive the destination");
    ok &= Check(g_construct_text ==
                    CCharString_ConstructCCoopSpiritDef_0066fa10_Name &&
                strcmp(g_construct_text, "CCoopSpiritDef") == 0,
                "constructor must receive the fixed class-name literal");
    ok &= Check(g_construct_length == -1,
                "constructor must receive sentinel length -1");
    ok &= Check(string.marker == 0x12345678,
                "constructor side effects must reach the destination");

    if (!ok)
        return 1;

    puts("PASS CCoopSpiritDef CCharString wrapper 0x0066FA10");
    return 0;
}
