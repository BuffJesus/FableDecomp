struct C3 { char pad[8]; int mode; };
struct B2 { char pad[4]; C3* c; };
struct A1 { char pad[0x210]; B2* b; };
int __fastcall GetCurrentMode(A1* self)
{
    return self->b->c->mode;
}