#include <cstdio>

unsigned long __fastcall CHeroTitleDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x50;
}

int main()
{
    unsigned long r = CHeroTitleDef_GetSizeofClass((void*)0);
    if (r == 0x50) {
        std::printf("CHeroTitleDef_0044c3a6_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}