struct EWeaponClass { int v; };
struct worker_t {
    // __fastcall member at 0x464931: takes (arg, &temp) on the stack; this in ecx.
    EWeaponClass* m(EWeaponClass* arg, void* pTemp);
};
struct vec { EWeaponClass* _First; worker_t* _Last; EWeaponClass* _End; };

EWeaponClass* __fastcall vec_Umove(vec* self, void* /*edx*/, EWeaponClass* arg)
{
    void* temp = 0;
    return self->_Last->m(arg, &temp);
}