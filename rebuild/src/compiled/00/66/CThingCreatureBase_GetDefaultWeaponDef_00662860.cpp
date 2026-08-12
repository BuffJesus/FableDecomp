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

long CThingCreatureBase::GetDefaultWeaponDef()
{
    SomeDef* d = this->def;
    if (d->flags & 0x10)
        return d->weaponA;
    return d->weaponB;
}