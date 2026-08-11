#pragma optimize("s",on)
// std::vector<EWeaponClass>::_Umove faithful reconstruction @ 005813bc
// Shape: return this->m_field4->Callee(param, &zeroed_local);

typedef int EWeaponClass;

struct Callee_t {
    // __fastcall method returning EWeaponClass*
    EWeaponClass* Do(EWeaponClass* p, EWeaponClass** out);
};

struct VecMove {
    void*      m_field0;   // +0
    Callee_t*  m_field4;   // +4
    void*      m_field8;   // +8
    EWeaponClass* _Umove(EWeaponClass* p);
};

EWeaponClass* VecMove::_Umove(EWeaponClass* p)
{
    EWeaponClass* local = 0;
    return this->m_field4->Do(p, &local);
}