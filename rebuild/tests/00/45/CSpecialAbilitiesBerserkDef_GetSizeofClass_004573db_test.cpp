#include <cstdio>

struct CSpecialAbilitiesBerserkDef {
    int placeholder;
};

unsigned long __fastcall CSpecialAbilitiesBerserkDef_GetSizeofClass(const CSpecialAbilitiesBerserkDef* self)
{
    (void)self;
    return 0x188;
}

int main()
{
    CSpecialAbilitiesBerserkDef obj;
    unsigned long r = CSpecialAbilitiesBerserkDef_GetSizeofClass(&obj);
    if (r != 0x188) {
        std::printf("FAIL: got %lu\n", r);
        return 1;
    }
    std::printf("CSpecialAbilitiesBerserkDef_004573db_TEST PASS\n");
    return 0;
}