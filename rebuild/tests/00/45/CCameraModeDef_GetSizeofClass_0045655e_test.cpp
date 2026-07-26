#include <cstdio>
struct CCameraModeDef;
unsigned long __fastcall CCameraModeDef_GetSizeofClass(CCameraModeDef* self)
{
    (void)self;
    return 0xd8;
}
int main()
{
    unsigned long r = CCameraModeDef_GetSizeofClass(0);
    if (r == 0xd8) { std::printf("CCameraModeDef_0045655e_TEST PASS\n"); return 0; }
    std::printf("FAIL got %lu\n", r); return 1;
}