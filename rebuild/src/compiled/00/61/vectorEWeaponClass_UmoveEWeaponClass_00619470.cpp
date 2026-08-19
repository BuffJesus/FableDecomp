// _Umove for vector<EWeaponClass> â€” STL uninitialized-move helper.
// Retail 0x00569af0. Forwards to inner copy/move at 0x5697c0.
typedef int EWeaponClass;

// Element/proxy object; the engine helper at 0x5697c0 is a __fastcall member
// (ecx = _Myfirst) taking two stack args (arg, &tmp). Reloc-masked call.
struct Elem {
    EWeaponClass* Move(EWeaponClass* arg, EWeaponClass* dest);
};

struct Vec {
    void* _Myproxy;   // +0
    Elem* _Myfirst;   // +4
    EWeaponClass* Inner(EWeaponClass* arg);
};

EWeaponClass* Vec::Inner(EWeaponClass* arg)
{
    EWeaponClass tmp = 0;
    return this->_Myfirst->Move(arg, &tmp);
}