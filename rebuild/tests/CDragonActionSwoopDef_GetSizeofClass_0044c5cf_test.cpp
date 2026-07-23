#include <cstdio>

unsigned long __fastcall CDragonActionSwoopDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x64;
}

int main()
{
    int dummy = 0;
    unsigned long r = CDragonActionSwoopDef_GetSizeofClass(&dummy);
    if (r == 0x64) {
        std::printf("CDragonActionSwoopDef_0044c5cf_TEST PASS\n");
        return 0;
    }
    std::printf("CDragonActionSwoopDef_0044c5cf_TEST FAIL got=%lu\n", r);
    return 1;
}