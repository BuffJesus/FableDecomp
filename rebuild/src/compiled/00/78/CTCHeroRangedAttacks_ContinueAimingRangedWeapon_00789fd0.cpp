struct CTCHeroRangedAttacks {
    char pad[0x14];
    unsigned char m14;
    unsigned char m15;
};

static bool __fastcall Helper(CTCHeroRangedAttacks* self);

void __fastcall ContinueAimingRangedWeapon(CTCHeroRangedAttacks* self)
{
    if (Helper(self)) {
        self->m14 = true;
        self->m15 = true;
    }
}

#pragma optimize("", off)
static bool __fastcall Helper(CTCHeroRangedAttacks* self)
{
    return self->m14 != 0;
}
#pragma optimize("", on)