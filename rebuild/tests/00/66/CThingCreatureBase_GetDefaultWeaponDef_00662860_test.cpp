#include <cstdio>

struct SomeDef {
    char pad0[0xd8];
    long weaponA;   // +0xd8
    long pad1;      // +0xdc
    long weaponB;   // +0xe0
    char pad2[0x148 - 0xe4];
    unsigned char flags; // +0x148
};

struct CThingCreatureBase {
    char pad0[0xe0];
    SomeDef* def;   // +0xe0
    long GetDefaultWeaponDef();
};

int main()
{
    SomeDef d;
    d.weaponA = 111;
    d.weaponB = 222;
    CThingCreatureBase c;
    c.def = &d;

    d.flags = 0x10;
    long r1 = c.GetDefaultWeaponDef();

    d.flags = 0x00;
    long r2 = c.GetDefaultWeaponDef();

    d.flags = 0xEF; // bit 0x10 clear, others set
    long r3 = c.GetDefaultWeaponDef();

    d.flags = 0x11; // bit 0x10 set
    long r4 = c.GetDefaultWeaponDef();

    if (r1 == 111 && r2 == 222 && r3 == 222 && r4 == 111) {
        printf("WEAPONDEF_OK\n");
        return 0;
    }
    printf("FAIL %ld %ld %ld %ld\n", r1, r2, r3, r4);
    return 1;
}