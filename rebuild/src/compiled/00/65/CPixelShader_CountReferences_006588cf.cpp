struct Inner { char pad[0xF8]; int refs; };
struct Outer { char pad[0x94]; Inner* inner; };
int __fastcall CountReferences(Outer* self)
{
    return self->inner->refs;
}