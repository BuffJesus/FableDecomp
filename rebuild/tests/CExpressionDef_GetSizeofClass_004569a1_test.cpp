#include <cstdio>
struct CExpressionDef {};
unsigned long __fastcall CExpressionDef_GetSizeofClass(const CExpressionDef* self)
{
    (void)self;
    return 0x90;
}
int main()
{
    CExpressionDef d;
    unsigned long r = CExpressionDef_GetSizeofClass(&d);
    if (r != 0x90) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CExpressionDef_004569a1_TEST PASS\n");
    return 0;
}