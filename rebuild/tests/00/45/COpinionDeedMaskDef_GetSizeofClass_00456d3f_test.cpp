#include <cstdio>
struct COpinionDeedMaskDef { char pad[0x48]; };
unsigned long __fastcall COpinionDeedMaskDef_GetSizeofClass(const COpinionDeedMaskDef* self)
{
    (void)self;
    return 0x48;
}
int main()
{
    COpinionDeedMaskDef obj;
    unsigned long r = COpinionDeedMaskDef_GetSizeofClass(&obj);
    if (r == 0x48) { std::printf("COpinionDeedMaskDef_00456d3f_TEST PASS\n"); return 0; }
    std::printf("FAIL got %lu\n", r); return 1;
}