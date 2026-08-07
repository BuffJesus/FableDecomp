struct S { char pad[0x18]; int* arr; };
int __fastcall GetAbilityLevel(S* self, void* edx, int index)
{
    (void)edx;
    return self->arr[index];
}