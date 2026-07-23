#include <cstdio>
typedef unsigned long ulong;

ulong __fastcall CJackDragonDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x78;
}

int main()
{
    if (CJackDragonDef_GetSizeofClass(0) == 0x78) {
        std::printf("CJackDragonDef_0044c63d_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}