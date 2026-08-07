struct Base { char pad[0x74]; float* arr; };
struct S { char pad0[0x0C]; Base* base; char pad1[0x0C]; int idx; };
float __fastcall GetDamageMultiplier(S* self)
{
    return self->base->arr[self->idx];
}