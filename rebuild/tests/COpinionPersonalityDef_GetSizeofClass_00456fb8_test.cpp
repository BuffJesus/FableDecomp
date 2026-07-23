#include <cstdio>

struct COpinionPersonalityDef { char _pad[0x54]; };

unsigned long __fastcall COpinionPersonalityDef_GetSizeofClass(const COpinionPersonalityDef* self)
{
    (void)self;
    return 0x54;
}

int main()
{
    COpinionPersonalityDef obj;
    unsigned long v = COpinionPersonalityDef_GetSizeofClass(&obj);
    if (v != 0x54) {
        std::printf("COpinionPersonalityDef_00456fb8_TEST FAIL got=%lu\n", v);
        return 1;
    }
    std::printf("COpinionPersonalityDef_00456fb8_TEST PASS\n");
    return 0;
}