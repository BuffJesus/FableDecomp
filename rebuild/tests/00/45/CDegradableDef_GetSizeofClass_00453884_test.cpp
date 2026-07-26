#include <cstdio>
typedef unsigned long ulong;
struct CDegradableDef {};
ulong __fastcall CDegradableDef_GetSizeofClass(CDegradableDef* self)
{
    (void)self;
    return 0x3c;
}
int main()
{
    CDegradableDef d;
    ulong r = CDegradableDef_GetSizeofClass(&d);
    if (r == 0x3c) {
        std::printf("CDegradableDef_00453884_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}