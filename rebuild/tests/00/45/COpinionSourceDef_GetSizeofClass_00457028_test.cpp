#include <cstdio>

unsigned long __fastcall COpinionSourceDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0xC8;
}

int main()
{
    unsigned long r = COpinionSourceDef_GetSizeofClass((void*)0x12345678);
    if (r == 0xC8) {
        std::printf("COpinionSourceDef_00457028_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}