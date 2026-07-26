#include <cstdio>

unsigned long __fastcall CFactionDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x54;
}

int main()
{
    unsigned long r = CFactionDef_GetSizeofClass((const void*)0x12345678);
    if (r == 0x54) {
        std::printf("CFactionDef_00456511_TEST PASS\n");
        return 0;
    }
    std::printf("CFactionDef_00456511_TEST FAIL got=%lu\n", r);
    return 1;
}