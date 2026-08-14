typedef int EWeaponClass;

struct Elem {
    EWeaponClass* Move(EWeaponClass* arg, EWeaponClass* dest);
};

struct Vec {
    void* _Myproxy;
    Elem* _Myfirst;
    EWeaponClass* Inner(EWeaponClass* arg);
};

EWeaponClass* Elem::Move(EWeaponClass* arg, EWeaponClass* dest)
{
    (void)arg;
    return dest;
}

EWeaponClass* Vec::Inner(EWeaponClass* arg)
{
    EWeaponClass tmp = 0;
    return this->_Myfirst->Move(arg, &tmp);
}

#include <cstdio>
int main()
{
    Elem e;
    Vec v; v._Myproxy = 0; v._Myfirst = &e;
    EWeaponClass a = 5;
    EWeaponClass* r = v.Inner(&a);
    printf("UMOVE_OK %d\n", (r != 0) ? 1 : 0);
    return 0;
}