#include <cstdio>
struct EWeaponClass { int v; };
struct vec { EWeaponClass* _dummy0; EWeaponClass* _Myfirst; };

static EWeaponClass* g_self; static EWeaponClass* g_a;
static EWeaponClass** g_b; static EWeaponClass* g_c;

EWeaponClass* __fastcall helper_4e8a87(EWeaponClass* self, EWeaponClass* a,
                                       EWeaponClass** b, EWeaponClass* c) {
    g_self=self; g_a=a; g_b=b; g_c=c; return c;
}

EWeaponClass* __fastcall vec_Umove(vec* self, void* edx, EWeaponClass* arg) {
    EWeaponClass* local = 0;
    return helper_4e8a87(self->_Myfirst, arg, &local, arg);
}

int main() {
    EWeaponClass arr[2]; vec v; v._dummy0=0; v._Myfirst=&arr[0];
    EWeaponClass* r = vec_Umove(&v, 0, &arr[1]);
    if (r==&arr[1] && g_self==&arr[0] && g_a==&arr[1] && g_c==&arr[1] && *g_b==0)
        printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}