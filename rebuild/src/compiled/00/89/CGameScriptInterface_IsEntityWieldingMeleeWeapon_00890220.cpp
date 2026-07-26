struct CEntity;
struct CGameScriptInterface;

struct CGSIVtbl {
    void* slot0[11];
    CEntity* (__fastcall* getEntity)(CGameScriptInterface* self); // +0x2c
};
struct CGameScriptInterface {
    CGSIVtbl* vt;
};
struct CEntity {
    unsigned char _pad[0x91];
    unsigned char flags; // +0x91
};

extern "C" bool __fastcall CheckMeleeWield(CEntity* e);

bool __stdcall CGameScriptInterface_IsEntityWieldingMeleeWeapon(CGameScriptInterface* self)
{
    CEntity* e = self->vt->getEntity(self);
    if (e != 0 && (e->flags & 1) == 0)
        return CheckMeleeWield(e);
    return false;
}