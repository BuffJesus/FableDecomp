struct Inner { char pad[0xC0]; int refs; };
struct Outer { char pad[0xE0]; Inner* inner; };
int __fastcall CountReferences(Outer* self)
{
    return self->inner->refs;
}