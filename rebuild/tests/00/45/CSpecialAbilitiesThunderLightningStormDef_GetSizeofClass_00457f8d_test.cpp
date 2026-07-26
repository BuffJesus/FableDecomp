#include <cstdio>

struct CSpecialAbilitiesThunderLightningStormDef { char pad[0x68]; };

unsigned long __fastcall CSpecialAbilitiesThunderLightningStormDef_GetSizeofClass(CSpecialAbilitiesThunderLightningStormDef* self)
{
    (void)self;
    return 0x68;
}

int main()
{
    CSpecialAbilitiesThunderLightningStormDef obj;
    unsigned long r = CSpecialAbilitiesThunderLightningStormDef_GetSizeofClass(&obj);
    if (r == 0x68) {
        std::printf("CSpecialAbilitiesThunderLightningStormDef_00457f8d_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL: got %lu\n", r);
    return 1;
}