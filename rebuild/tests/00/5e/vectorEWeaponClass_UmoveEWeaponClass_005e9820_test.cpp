#include <cstdio>
struct EWeaponClass { int v; };
struct worker_t {
    // stand-in for the 0x464931 copy/move worker: returns the dest cursor.
    EWeaponClass* m(EWeaponClass* arg, void* pTemp);
};
struct vec { EWeaponClass* _First; worker_t* _Last; EWeaponClass* _End; };

static worker_t* g_this;
static EWeaponClass* g_arg;
static void* g_temp;
EWeaponClass* worker_t::m(EWeaponClass* arg, void* pTemp){
    g_this = this; g_arg = arg; g_temp = pTemp;
    return arg;
}

EWeaponClass* __fastcall vec_Umove(vec* self, void* /*edx*/, EWeaponClass* arg){
    void* temp = 0;
    return self->_Last->m(arg, &temp);
}

int main(){
    worker_t w;
    EWeaponClass a;
    vec v; v._First=0; v._Last=&w; v._End=0;
    EWeaponClass* r = vec_Umove(&v, 0, &a);
    if (r == &a && g_this == &w && g_arg == &a && g_temp != 0) printf("BEHAVIOR_OK\n");
    else printf("BEHAVIOR_FAIL\n");
    return 0;
}