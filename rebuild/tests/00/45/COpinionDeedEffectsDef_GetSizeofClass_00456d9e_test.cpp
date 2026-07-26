#include <cstdio>

struct COpinionDeedEffectsDef { int dummy; };

unsigned long __fastcall COpinionDeedEffectsDef_GetSizeofClass(const COpinionDeedEffectsDef* self)
{
    (void)self;
    return 0x80;
}

int main()
{
    COpinionDeedEffectsDef obj;
    unsigned long r = COpinionDeedEffectsDef_GetSizeofClass(&obj);
    if (r == 0x80) {
        std::printf("COpinionDeedEffectsDef_00456d9e_TEST PASS\n");
        return 0;
    }
    std::printf("COpinionDeedEffectsDef_00456d9e_TEST FAIL got %lu\n", r);
    return 1;
}