#pragma optimize("t",on)

struct CCreatureAction_FireMissileWeapon {
    char pad[0xC0];
    int cooldown;
};

extern "C" void __fastcall BaseFrameUpdate(CCreatureAction_FireMissileWeapon* self);

extern "C" void __fastcall FrameUpdate(CCreatureAction_FireMissileWeapon* self)
{
    int c = self->cooldown;
    if (c == 0) {
        BaseFrameUpdate(self);
        return;
    }
    self->cooldown = c - 1;
}