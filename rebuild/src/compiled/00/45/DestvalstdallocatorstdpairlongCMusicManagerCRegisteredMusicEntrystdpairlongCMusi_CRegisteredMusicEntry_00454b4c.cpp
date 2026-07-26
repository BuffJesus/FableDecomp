struct Inner { void dtor(); };
struct Pair { char pad[0x39]; Inner inner; void dtor(); };

void __fastcall Dest_val(Pair* self)
{
    self->inner.dtor();
    self->dtor();
}