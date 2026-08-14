struct EWeaponClass { int v; };

struct Alloc {
    EWeaponClass* mv(EWeaponClass* first, unsigned int* marker);
};

struct VecBase {
    void*  _m0;
    Alloc* _al;
};

EWeaponClass* __fastcall _Umove_EWeaponClass(VecBase* self, void* /*edx*/, EWeaponClass* first)
{
    unsigned int marker = 0xffffffff;
    return self->_al->mv(first, &marker);
}