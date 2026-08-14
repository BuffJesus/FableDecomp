// std::vector<EWeaponClass>::_Umove-ish reconstruction @ 0x0074ddb0
struct EWeaponClass { int v; };

// The move helper at 0x4e8a87 is a __fastcall member on the element pointer
// block: this=ecx (= vector::_Myfirst), two stack args (source, &dest_local).
struct EWBlock {
    EWeaponClass* helper_4e8a87(EWeaponClass* arg, EWeaponClass** dest);
};

struct vec {
    EWeaponClass* _dummy0;   // +0
    EWeaponClass* _Myfirst;  // +4  -> passed as `this` (ecx) to the helper
};

EWeaponClass* __fastcall vec_Umove(vec* self /*ecx*/, void* edx_dummy,
                                   EWeaponClass* arg)
{
    EWeaponClass* local = 0;
    return ((EWBlock*)self->_Myfirst)->helper_4e8a87(arg, &local);
}