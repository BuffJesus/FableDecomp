#include <cstdio>

struct CEnvironmentThemeDaySetDef { char pad[0x6c]; };

unsigned long __fastcall CEnvironmentThemeDaySetDef_GetSizeofClass(const CEnvironmentThemeDaySetDef* self)
{
    (void)self;
    return sizeof(CEnvironmentThemeDaySetDef);
}

int main()
{
    CEnvironmentThemeDaySetDef obj;
    unsigned long r = CEnvironmentThemeDaySetDef_GetSizeofClass(&obj);
    if (r == 0x6c) {
        std::printf("CEnvironmentThemeDaySetDef_00455c60_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}