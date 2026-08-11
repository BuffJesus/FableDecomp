#include <cstdio>

struct EWeaponClass;

struct AllocT {
    EWeaponClass* construct(EWeaponClass* p, char* local);
};

EWeaponClass* __fastcall _Umove_EWeaponClass(AllocT* self, int edx_unused, EWeaponClass* p);

static char* g_seen_local = 0;
static EWeaponClass* g_seen_p = 0;
static AllocT* g_seen_self = 0;

EWeaponClass* AllocT::construct(EWeaponClass* p, char* local)
{
    g_seen_self = this;
    g_seen_p = p;
    g_seen_local = local;
    *local = 7; /* prove local is writable stack */
    return p;
}

int main()
{
    AllocT a;
    EWeaponClass* fake = (EWeaponClass*)0x1234;
    EWeaponClass* r = _Umove_EWeaponClass(&a, 0, fake);
    if (r == fake && g_seen_self == &a && g_seen_p == fake && g_seen_local != 0)
        printf("005790fc_TEST PASS\n");
    else
        printf("FAIL\n");
    return 0;
}