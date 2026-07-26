#include <cstdio>
unsigned long __fastcall CEnvironmentDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x134;
}
int main()
{
    if (CEnvironmentDef_GetSizeofClass(0) == 0x134) {
        std::printf("CEnvironmentDef_004558d9_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}