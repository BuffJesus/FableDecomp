#include <cstdio>

struct CHeroPostcardGeneratorDef {
    int _pad;
};

unsigned long __fastcall CHeroPostcardGeneratorDef_GetSizeofClass(const CHeroPostcardGeneratorDef* self)
{
    (void)self;
    return 0x5c;
}

int main()
{
    CHeroPostcardGeneratorDef obj;
    unsigned long r = CHeroPostcardGeneratorDef_GetSizeofClass(&obj);
    if (r == 0x5c) {
        std::printf("CHeroPostcardGeneratorDef_0045687f_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got=%lu\n", r);
    return 1;
}