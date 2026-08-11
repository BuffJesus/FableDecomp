#include <cstdio>

typedef int EWeaponClass;

struct Callee_t {
    EWeaponClass* Do(EWeaponClass* p, EWeaponClass** out);
};

struct VecMove {
    void*      m_field0;
    Callee_t*  m_field4;
    void*      m_field8;
    EWeaponClass* _Umove(EWeaponClass* p);
};

// Observe the arguments passed by _Umove.
static EWeaponClass* g_seenP;
static EWeaponClass** g_seenOut;
static Callee_t* g_seenThis;

EWeaponClass* Callee_t::Do(EWeaponClass* p, EWeaponClass** out)
{
    g_seenThis = this;
    g_seenP = p;
    g_seenOut = out;
    // The temp *out must have been zeroed by the caller.
    return (EWeaponClass*)((char*)0 + (*out == 0 ? 0x1234 : 0xBAD));
}

EWeaponClass* VecMove::_Umove(EWeaponClass* p)
{
    EWeaponClass* local = 0;
    return this->m_field4->Do(p, &local);
}

int main()
{
    Callee_t callee;
    VecMove v;
    v.m_field0 = 0;
    v.m_field4 = &callee;
    v.m_field8 = 0;

    EWeaponClass dummy = 7;
    EWeaponClass* r = v._Umove(&dummy);

    bool ok = true;
    if (g_seenThis != &callee) ok = false;   // ecx = this->m_field4
    if (g_seenP != &dummy) ok = false;        // arg1 = param
    if (g_seenOut == 0) ok = false;           // arg2 = &local (on stack, non-null)
    if (r != (EWeaponClass*)0x1234) ok = false; // local was zeroed

    printf("this=%p p=%p out=%p r=%p\n", (void*)g_seenThis, (void*)g_seenP, (void*)g_seenOut, (void*)r);
    if (ok) printf("UMOVE_PARITY_OK\n");
    else printf("UMOVE_FAIL\n");
    return ok ? 0 : 1;
}