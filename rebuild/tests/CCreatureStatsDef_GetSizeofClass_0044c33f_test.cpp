#include <cstdio>
struct CCreatureStatsDef {};
unsigned long __fastcall CCreatureStatsDef_GetSizeofClass(CCreatureStatsDef* self)
{
    (void)self;
    return 0x48;
}
int main()
{
    CCreatureStatsDef d;
    unsigned long r = CCreatureStatsDef_GetSizeofClass(&d);
    if (r == 0x48UL) {
        std::printf("CCreatureStatsDef_0044c33f_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}