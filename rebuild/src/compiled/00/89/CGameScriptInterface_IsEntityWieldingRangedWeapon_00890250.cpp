struct Entity;
struct GSI_vt {
    void* slot[0xb]; // 0x00..0x28
    Entity* (__fastcall *getEntity)(void*); // +0x2c
};
struct Entity {
    unsigned char _pad[0x91];
    unsigned char flags; // +0x91
};
extern bool __fastcall EntityWieldsRanged(Entity* e);

bool __stdcall CGameScriptInterface_IsEntityWieldingRangedWeapon(void* self)
{
    GSI_vt** vt = (GSI_vt**)self;
    Entity* e = (*vt)->getEntity(self);
    if (e != 0 && (((*(unsigned char*)((char*)e + 0x91)) & 1) == 0))
        return EntityWieldsRanged(e);
    return false;
}