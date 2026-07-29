#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
ZeroFields_0C_10_14_18_1C_20_24_0066fa30(void* object);

struct Object
{
    unsigned long field_00;
    unsigned long field_04;
    unsigned long field_08;
    unsigned long field_0c;
    unsigned long field_10;
    unsigned long field_14;
    unsigned long field_18;
    unsigned long field_1c;
    unsigned long field_20;
    unsigned long field_24;
    unsigned long field_28;
};

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
    Object object;

    memset(&object, 0x5a, sizeof(object));
    ZeroFields_0C_10_14_18_1C_20_24_0066fa30(&object);

    ok &= Check(object.field_0c == 0 &&
                object.field_10 == 0 &&
                object.field_14 == 0 &&
                object.field_18 == 0 &&
                object.field_1c == 0 &&
                object.field_20 == 0 &&
                object.field_24 == 0,
                "initializer must clear all seven recovered fields");
    ok &= Check(object.field_00 == 0x5a5a5a5a &&
                object.field_04 == 0x5a5a5a5a &&
                object.field_08 == 0x5a5a5a5a &&
                object.field_28 == 0x5a5a5a5a,
                "initializer must preserve adjacent object state");

    object.field_0c = 1;
    object.field_10 = 2;
    object.field_14 = 3;
    object.field_18 = 4;
    object.field_1c = 5;
    object.field_20 = 6;
    object.field_24 = 7;
    ZeroFields_0C_10_14_18_1C_20_24_0066fa30(&object);
    ok &= Check(object.field_0c == 0 &&
                object.field_10 == 0 &&
                object.field_14 == 0 &&
                object.field_18 == 0 &&
                object.field_1c == 0 &&
                object.field_20 == 0 &&
                object.field_24 == 0,
                "initializer must overwrite nonzero field values");

    if (!ok)
        return 1;

    puts("PASS seven-field zero initializer 0x0066FA30");
    return 0;
}
