#include <stdio.h>

struct Object_0066fb40
{
    unsigned char prefix[0x28];
    unsigned long field_28;
};

extern "C" unsigned long __fastcall
ReturnField28_0066fb40(const Object_0066fb40* self);

int main()
{
    Object_0066fb40 object;

    object.field_28 = 0x12345678;
    if (ReturnField28_0066fb40(&object) != 0x12345678)
    {
        puts("FAIL: getter must return all bits from offset 0x28");
        return 1;
    }

    object.field_28 = 0;
    if (ReturnField28_0066fb40(&object) != 0)
    {
        puts("FAIL: getter must preserve zero");
        return 1;
    }

    object.field_28 = 0xffffffff;
    if (ReturnField28_0066fb40(&object) != 0xffffffff)
    {
        puts("FAIL: getter must preserve the high bit");
        return 1;
    }

    puts("PASS field-0x28 getter 0x0066FB40");
    return 0;
}
