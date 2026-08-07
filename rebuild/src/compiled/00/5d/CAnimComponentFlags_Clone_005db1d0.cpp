struct S; typedef S* (__fastcall* VFn)(S*);
struct S { VFn* vtbl; char pad[4]; void* f8; };
S* __fastcall Clone(S* self){ S* p = self->vtbl[0x10/4](self); p->f8 = self->f8; return p; }