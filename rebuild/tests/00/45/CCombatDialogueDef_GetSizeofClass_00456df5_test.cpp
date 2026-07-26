#include <cstdio>

typedef unsigned long ulong;

ulong __fastcall CCombatDialogueDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x8c;
}

int main()
{
    ulong r = CCombatDialogueDef_GetSizeofClass((void*)0x12345678);
    if (r == 0x8c) {
        std::printf("CCombatDialogueDef_00456df5_TEST PASS\n");
        return 0;
    }
    std::printf("CCombatDialogueDef_00456df5_TEST FAIL got %lu\n", r);
    return 1;
}