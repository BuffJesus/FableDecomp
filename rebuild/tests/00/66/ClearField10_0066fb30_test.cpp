#include <stdio.h>
#include <string.h>

struct Object_0066fb30
{
    unsigned char prefix[0x10];
    void* field_10;
    unsigned long suffix;
};

extern "C" void __fastcall
ClearField10_0066fb30(Object_0066fb30* self);

int main()
{
    Object_0066fb30 object;
    unsigned long marker = 0x12345678;

    memset(&object, 0x5a, sizeof(object));
    object.field_10 = &marker;
    ClearField10_0066fb30(&object);

    if (object.field_10 != 0)
    {
        puts("FAIL: field at offset 0x10 must be cleared");
        return 1;
    }

    if (object.prefix[0] != 0x5a ||
        object.prefix[0x0f] != 0x5a ||
        object.suffix != 0x5a5a5a5a)
    {
        puts("FAIL: clear must preserve adjacent object state");
        return 1;
    }

    ClearField10_0066fb30(&object);
    if (object.field_10 != 0)
    {
        puts("FAIL: clearing an already-null field must be stable");
        return 1;
    }

    puts("PASS field-0x10 clear 0x0066FB30");
    return 0;
}
