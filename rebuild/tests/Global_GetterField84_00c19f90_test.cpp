#include <stdio.h>
#include <string.h>

struct CObj
{
    unsigned char pad00[0x84];
    unsigned int  field84;   // +0x84
};

unsigned int __fastcall Getter_Field84(struct CObj* self)
{
    return self->field84;
}

int main()
{
    CObj obj;
    memset(&obj, 0, sizeof(obj));
    obj.field84 = 0xDEADBEEFu;

    // Poison neighbours to ensure we read exactly +0x84.
    unsigned int got = Getter_Field84(&obj);

    if (got == 0xDEADBEEFu &&
        ((size_t)&obj.field84 - (size_t)&obj) == 0x84)
    {
        printf("GETTER_FIELD84_TEST PASS\n");
        return 0;
    }
    printf("GETTER_FIELD84_TEST FAIL got=%08X\n", got);
    return 1;
}