#include <stdio.h>

typedef short EWeaponClass;

static EWeaponClass* g_lastArg = 0;
static void*         g_lastSelf = 0;

struct SubObj {
    EWeaponClass* call(EWeaponClass* p);
};

struct WVec {
    char pad0[4];
    EWeaponClass member4;
    char pad6[2];
    SubObj sub;
};

EWeaponClass* SubObj::call(EWeaponClass* p)
{
    g_lastSelf = this;
    g_lastArg = p;
    return p;
}

EWeaponClass* __fastcall vector_EWeaponClass_Umove(WVec* self, int /*edx*/,
                                                   EWeaponClass a, EWeaponClass* b)
{
    self->member4 = a;
    return self->sub.call(b);
}

int main()
{
    WVec v;
    v.member4 = 0;
    EWeaponClass target = 77;
    EWeaponClass* ret = vector_EWeaponClass_Umove(&v, 0, (EWeaponClass)0x1234, &target);

    bool ok = true;
    if (v.member4 != (EWeaponClass)0x1234) ok = false;   // stored word at +4
    if (ret != &target) ok = false;                       // forwarded ptr returned
    if (g_lastArg != &target) ok = false;
    if ((char*)g_lastSelf != (char*)&v + 8) ok = false;   // this+8 sub-object

    printf(ok ? "UMOVE_OK\n" : "UMOVE_FAIL\n");
    return ok ? 0 : 1;
}