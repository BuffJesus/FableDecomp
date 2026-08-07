struct A { char pad[232]; float* arr; };
struct S { char pad[0xc]; A* f0c; char pad2[80-0x10]; int idx; };
float __fastcall GetDamageMultiplier(S* self){ A* a = self->f0c; return a->arr[self->idx]; }