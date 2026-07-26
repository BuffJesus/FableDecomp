#include <cstdio>

unsigned long __fastcall CTattooDef_GetSizeofClass(void* self)
{
    return 0x60;
}

int main()
{
    unsigned long r = CTattooDef_GetSizeofClass(0);
    if (r == 0x60) {
        std::printf("CTattooDef_0044c3d4_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}