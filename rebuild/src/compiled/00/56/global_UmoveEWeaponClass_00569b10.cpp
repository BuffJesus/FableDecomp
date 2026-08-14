struct EWeaponClass;

struct Dest {
    EWeaponClass* copy(EWeaponClass* first, int* local);
};

struct Vec {
    void* pad0;
    Dest* dest;
};

EWeaponClass* __fastcall vector_Umove(Vec* self, int edx_unused, EWeaponClass* first)
{
    int local = 0;
    return self->dest->copy(first, &local);
}