#include <cstdio>

struct CGuildMasterDef { int _pad; };

unsigned long __fastcall CGuildMasterDef_GetSizeofClass(const CGuildMasterDef* self)
{
    (void)self;
    return 0x30;
}

int main()
{
    CGuildMasterDef d;
    unsigned long r = CGuildMasterDef_GetSizeofClass(&d);
    if (r != 0x30) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CGuildMasterDef_0044c67f_TEST PASS\n");
    return 0;
}