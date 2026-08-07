struct A { char pad[0x380]; void** arr; };
struct S { char pad[4]; A* f4; char pad2[0x3c-8]; int idx; };
void* __fastcall PeekPhysicsMesh(S* self){ A* a=self->f4; return a->arr[self->idx]; }