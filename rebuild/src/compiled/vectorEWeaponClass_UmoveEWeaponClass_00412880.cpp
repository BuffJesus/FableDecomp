struct EWeaponClass;

struct AllocT {
    EWeaponClass* construct(EWeaponClass* p, char* local);
};

EWeaponClass* __fastcall vector_Umove(AllocT* self, int edx_unused, EWeaponClass* p)
{
    char local = 0;
    return self->construct(p, &local);
}