#include <cstdio>

unsigned long __fastcall CAugmentationDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x4c;
}

int main()
{
    int dummy = 0;
    unsigned long r = CAugmentationDef_GetSizeofClass(&dummy);
    if (r == 0x4c) {
        std::printf("CAugmentationDef_0044c611_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}