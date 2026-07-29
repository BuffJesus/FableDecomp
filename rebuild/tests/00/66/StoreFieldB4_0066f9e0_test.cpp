#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
StoreFieldB4_0066f9e0(
    void* object, void*, unsigned long value);

struct Object
{
    unsigned char prefix[0xb4];
    unsigned long field_b4;
    unsigned long suffix;
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
    StoreFieldB4_0066f9e0(&object, 0, 0x12345678);
    ok &= Check(object.field_b4 == 0x12345678,
                "setter must store the argument at offset 0xB4");
    ok &= Check(object.prefix[0] == 0x5a &&
                object.prefix[0xb3] == 0x5a &&
                object.suffix == 0x5a5a5a5a,
                "setter must preserve adjacent object state");

    StoreFieldB4_0066f9e0(&object, 0, 0);
    ok &= Check(object.field_b4 == 0,
                "setter must preserve a zero value");

    StoreFieldB4_0066f9e0(&object, 0, 0xffffffff);
    ok &= Check(object.field_b4 == 0xffffffff,
                "setter must preserve all argument bits");

    if (!ok)
        return 1;

    puts("PASS field-0xB4 setter 0x0066F9E0");
    return 0;
}
