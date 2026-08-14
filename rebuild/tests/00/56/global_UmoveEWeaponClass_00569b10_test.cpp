#include <cstdio>

struct EWeaponClass;

struct Dest {
    EWeaponClass* copy(EWeaponClass* first, int* local);
};

struct Vec {
    void* pad0;
    Dest* dest;
};

EWeaponClass* __fastcall vector_Umove(Vec* self, int edx_unused, EWeaponClass* first);

static int g_local_seen = -1;
static EWeaponClass* g_first_seen = 0;
static Dest* g_this_seen = 0;

EWeaponClass* Dest::copy(EWeaponClass* first, int* local)
{
    g_this_seen = this;
    g_first_seen = first;
    g_local_seen = *local;
    return first;
}

int main()
{
    Dest d;
    Vec v;
    v.pad0 = (void*)0xdead;
    v.dest = &d;
    EWeaponClass* fake = (EWeaponClass*)0x1234;
    EWeaponClass* r = vector_Umove(&v, 0, fake);
    if (r == fake && g_first_seen == fake && g_local_seen == 0 && g_this_seen == &d)
        printf("UMOVE_EWC_OK\n");
    else
        printf("UMOVE_EWC_FAIL\n");
    return 0;
}