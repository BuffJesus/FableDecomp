#include <cstdio>
typedef unsigned long ulong;
struct CSpecialAbilitiesGhostSwordDef {};
ulong __fastcall CSpecialAbilitiesGhostSwordDef_GetSizeofClass(CSpecialAbilitiesGhostSwordDef* self)
{
    (void)self;
    return 0x58;
}
int main()
{
    CSpecialAbilitiesGhostSwordDef obj;
    ulong v = CSpecialAbilitiesGhostSwordDef_GetSizeofClass(&obj);
    if (v == 0x58) {
        std::printf("CSpecialAbilitiesGhostSwordDef_00457a09_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", v);
    return 1;
}