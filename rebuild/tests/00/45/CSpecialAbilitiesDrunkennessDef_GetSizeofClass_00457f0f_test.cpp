#include <cstdio>

struct CSpecialAbilitiesDrunkennessDef { int dummy; };

unsigned long __fastcall CSpecialAbilitiesDrunkennessDef_GetSizeofClass(const CSpecialAbilitiesDrunkennessDef* self)
{
    (void)self;
    return 0x50;
}

int main()
{
    CSpecialAbilitiesDrunkennessDef obj;
    unsigned long r = CSpecialAbilitiesDrunkennessDef_GetSizeofClass(&obj);
    if (r != 0x50) {
        std::printf("FAIL: got %lu expected 80\n", r);
        return 1;
    }
    std::printf("CSpecialAbilitiesDrunkennessDef_00457f0f_TEST PASS\n");
    return 0;
}