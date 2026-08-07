struct Pair { int a; int b; };
struct FD { char pad[0x14]; int f14; int f18; };
Pair* __fastcall fn_00b3a990(FD* self, int _edx, Pair* out){ out->a=self->f14; out->b=self->f18; return out; }