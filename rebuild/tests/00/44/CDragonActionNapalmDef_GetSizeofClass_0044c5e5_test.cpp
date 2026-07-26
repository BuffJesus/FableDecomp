#include <cstdio>
typedef unsigned long ulong;

ulong __fastcall CDragonActionNapalmDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x38;
}

int main()
{
    ulong r = CDragonActionNapalmDef_GetSizeofClass((const void*)0);
    if (r != 0x38) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CDragonActionNapalmDef_0044c5e5_TEST PASS\n");
    return 0;
}