struct Inner { char pad[0x86]; unsigned char f; };
struct S { char pad[0xA8]; Inner* p; };
unsigned char __fastcall IsToReplaceItemIfRemovedExt(S* self)
{
    return self->p->f;
}