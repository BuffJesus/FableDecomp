struct S { char pad[4]; void* f4; void* f8; };
void* __fastcall DrawGetWorldMap(S* self){ if(!self->f4) return (void*)-1; return self->f8; }