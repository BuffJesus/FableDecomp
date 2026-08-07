struct S { void (__fastcall **vptr)(S*); };
void __fastcall Activate(S* self){ self->vptr[0x14/4](self); self->vptr[0x18/4](self); }