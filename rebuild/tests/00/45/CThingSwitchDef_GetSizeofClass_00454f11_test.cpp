#include <cstdio>
unsigned long __fastcall CThingSwitchDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x74;
}
int main()
{
    unsigned long r = CThingSwitchDef_GetSizeofClass(0);
    if (r == 0x74) { std::printf("CThingSwitchDef_00454f11_TEST PASS\n"); return 0; }
    std::printf("FAIL got %lu\n", r);
    return 1;
}