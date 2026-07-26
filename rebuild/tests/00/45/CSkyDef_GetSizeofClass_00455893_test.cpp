#include <cstdio>
struct CSkyDef;
unsigned long __fastcall CSkyDef_GetSizeofClass(const CSkyDef* self)
{
    (void)self;
    return 0x80;
}
int main()
{
    unsigned long r = CSkyDef_GetSizeofClass(0);
    if (r == 0x80) { std::printf("CSkyDef_00455893_TEST PASS\n"); return 0; }
    std::printf("FAIL got %lu\n", r); return 1;
}