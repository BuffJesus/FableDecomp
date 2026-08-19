// std::_Umove for vector<EWeaponClass>
// Retail 0x00a98600 __fastcall, ret 4 (one stack arg used)
typedef int EWeaponClass;

struct Iter8 { EWeaponClass* a; EWeaponClass* b; };

// callee 0xa98540: __fastcall(self_ecx; stack: first, Iter8* tmp) -> returns pointer.
// Modeled as __fastcall so self stays in ecx; two stack args pushed right-to-left.
extern EWeaponClass* __fastcall callee_a98540(void* self, int edx, int first, Iter8* tmp);

EWeaponClass* __fastcall f(void* self, int /*edx*/, int first)
{
    Iter8 tmp;
    tmp.a = 0;
    tmp.b = 0;
    return callee_a98540(self, first, first, &tmp);
}