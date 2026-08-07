struct S { char pad[0x1d4]; void* f1d4; };
bool __fastcall HasPhysicsMesh(S* self){ return self->f1d4 != 0; }