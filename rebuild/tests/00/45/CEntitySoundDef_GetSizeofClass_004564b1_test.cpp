#include <cstdio>
struct CEntitySoundDef;
unsigned long __fastcall CEntitySoundDef_GetSizeofClass(const CEntitySoundDef* self)
{
    (void)self;
    return 0x54;
}
int main()
{
    unsigned long r = CEntitySoundDef_GetSizeofClass(0);
    if (r == 0x54) {
        std::printf("CEntitySoundDef_004564b1_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}