#pragma optimize("s",on)
// std::vector<EWeaponClass>::_Umove reconstruction @ 0x00579f8a
struct EWeaponClass { int v; };
struct Temp { EWeaponClass* a; EWeaponClass* b; EWeaponClass* c; };
struct Vec { EWeaponClass* _First; EWeaponClass* _Last; EWeaponClass* _End; };

extern "C" void* __cdecl memset(void*, int, unsigned int);
#pragma intrinsic(memset)

// helper at 0x579ef5: __stdcall taking (Vec* self, Temp* t) -> EWeaponClass*
EWeaponClass* __stdcall helper(Vec* self, Temp* t);

EWeaponClass* __stdcall _Umove_EWeaponClass_(Vec* self)
{
    Temp t;
    memset(&t, 0, sizeof(Temp));
    return helper(self, &t);
}