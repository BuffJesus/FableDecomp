#include <cstdio>

unsigned long __fastcall CContextSensitiveItemDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x3c;
}

int main()
{
    int dummy = 0;
    unsigned long r = CContextSensitiveItemDef_GetSizeofClass(&dummy);
    if (r != 0x3c) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CContextSensitiveItemDef_0044c37a_TEST PASS\n");
    return 0;
}