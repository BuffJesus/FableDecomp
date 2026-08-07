struct S { char pad[4]; void* f4; char pad2[4]; void* f0c; };
void* __fastcall DrawGetWorldMap(S* self){ if(!self->f4) return 0; return self->f0c; }