struct Inner { char pad[0x86]; unsigned char f; };
struct S { char pad[0xA8]; Inner* p; };
unsigned char __fastcall IsToReplaceItemIfRemoved(S* self)
{
    return self->p->f;
}