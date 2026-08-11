#include <cstdio>

struct CThingGameObject {
    char pad[0xc8];
    unsigned char flags;
    void SetAddToComboMultiplierOnHit(bool v);
};

void CThingGameObject::SetAddToComboMultiplierOnHit(bool v)
{
    flags = (unsigned char)((flags & 0xfe) | (v & 1) | 2);
}

int main()
{
    CThingGameObject o;
    o.flags = 0x00;
    o.SetAddToComboMultiplierOnHit(true);
    // 0 -> (0&fe)|1|2 = 3
    if (o.flags != 0x03) { printf("FAIL a %d\n", o.flags); return 1; }

    o.flags = 0xff;
    o.SetAddToComboMultiplierOnHit(false);
    // ff -> (ff&fe)|0|2 = fe
    if (o.flags != 0xfe) { printf("FAIL b %d\n", o.flags); return 1; }

    o.flags = 0x55;
    o.SetAddToComboMultiplierOnHit(true);
    // 55 = 0101 0101 -> &fe=54 |1=55 |2=57
    if (o.flags != 0x57) { printf("FAIL c %d\n", o.flags); return 1; }

    printf("COMBO_MULT_OK\n");
    return 0;
}