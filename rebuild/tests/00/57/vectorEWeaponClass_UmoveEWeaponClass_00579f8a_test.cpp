#include <cstdio>
struct EWeaponClass { int v; };
struct Temp { EWeaponClass* a; EWeaponClass* b; EWeaponClass* c; };
struct Vec { EWeaponClass* _First; EWeaponClass* _Last; EWeaponClass* _End; };

static Temp g_seen;
static Vec* g_self;

EWeaponClass* __fastcall helper(Vec* self, void* edx, Temp* t){
    g_self = self; g_seen = *t;
    return self ? self->_First : 0;
}

EWeaponClass* __fastcall _Umove_EWeaponClass_(Vec* self, void* edx,
        EWeaponClass* first, EWeaponClass* last, EWeaponClass* dest){
    Temp t; t.a=0; t.b=0; t.c=0;
    return helper(self, 0, &t);
}

int main(){
    EWeaponClass arr[2]; Vec v; v._First=arr; v._Last=arr+2; v._End=arr+2;
    EWeaponClass* r = _Umove_EWeaponClass_(&v, 0, 0, 0, 0);
    int ok = (r==arr) && (g_self==&v) && (g_seen.a==0 && g_seen.b==0 && g_seen.c==0);
    if(ok) printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}