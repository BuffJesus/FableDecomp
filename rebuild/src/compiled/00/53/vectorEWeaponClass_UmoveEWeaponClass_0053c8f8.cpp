#pragma optimize("s",on)
struct EWeaponClass;

struct AllocT {
    EWeaponClass* construct(EWeaponClass* p, char* local);
};

EWeaponClass* __fastcall _Umove_EWeaponClass(AllocT* self, int edx_unused, EWeaponClass* p)
{
    char local;
    return self->construct(p, &local);
}