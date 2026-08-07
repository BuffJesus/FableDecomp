struct Inner { char pad[0x85]; unsigned char f; };
struct S { char pad[0xA8]; Inner* p; };
unsigned char __fastcall IsToHaveEmptyHands(S* self)
{
    return self->p->f;
}