struct CThingGameObject {
    char pad[0xc8];
    unsigned char flags;
};

void __fastcall SetAsGivingHeroStatChangesOnBeingHit(CThingGameObject *self, int edx_dummy, bool value)
{
    self->flags = (unsigned char)((((value & 1) | 2) << 2) | (self->flags & 0xfb));
}