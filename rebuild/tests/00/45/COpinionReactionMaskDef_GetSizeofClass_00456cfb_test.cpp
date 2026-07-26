#include <cstdio>
struct COpinionReactionMaskDef { int _pad; };
unsigned long __fastcall COpinionReactionMaskDef_GetSizeofClass(const COpinionReactionMaskDef* self)
{
    (void)self;
    return 0x48;
}
int main()
{
    COpinionReactionMaskDef o;
    if (COpinionReactionMaskDef_GetSizeofClass(&o) == 0x48u) {
        std::printf("COpinionReactionMaskDef_00456cfb_TEST PASS\n");
        return 0;
    }
    std::printf("COpinionReactionMaskDef_00456cfb_TEST FAIL\n");
    return 1;
}