struct S { char pad[0xF8]; void* fF8; };
bool __fastcall op_call(S* self, void* entry){ (void)entry; return self->fF8 == 0; }