struct S { char pad[0x24]; int* arr; };
int __fastcall GetAbilityLevel(S* self, void* edx, int index)
{
    (void)edx;
    return self->arr[index];
}