struct A { char pad[0x84]; float* arr; };
struct S { char pad[0xc]; A* f0c; int idx; };
float __fastcall GetDamageMultiplier(S* self){ A* a = self->f0c; return a->arr[self->idx]; }