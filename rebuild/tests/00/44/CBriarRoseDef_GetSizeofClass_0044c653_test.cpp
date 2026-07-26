#include <cstdio>

unsigned long __fastcall CBriarRoseDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x54;
}

int main()
{
    unsigned long r = CBriarRoseDef_GetSizeofClass((void*)0x12345678);
    if (r == 0x54) {
        std::printf("CBriarRoseDef_0044c653_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}