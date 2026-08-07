struct S { char pad[0xD8]; unsigned char f; };
unsigned char __fastcall PopDontPopulateNextLoadedRegion(S* self)
{
    unsigned char v = self->f;
    self->f = 0;
    return v;
}