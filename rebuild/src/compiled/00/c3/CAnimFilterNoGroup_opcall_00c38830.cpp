struct S { char pad[0xF8]; int fF8; };
bool __fastcall op_call(S* self, void* e){ (void)e; return self->fF8 == 6; }