#include <cstdio>

struct CSoundThemeDef { char pad[0x64]; };

unsigned long __fastcall CSoundThemeDef_GetSizeofClass(const CSoundThemeDef* self)
{
    return 0x64;
}

int main()
{
    CSoundThemeDef d;
    unsigned long r = CSoundThemeDef_GetSizeofClass(&d);
    if (r == 0x64) {
        std::printf("CSoundThemeDef_00455ec6_TEST PASS\n");
        return 0;
    }
    std::printf("CSoundThemeDef_00455ec6_TEST FAIL got %lu\n", r);
    return 1;
}