// std::vector<EWeaponClass>::_Umove helper @ 0x00c27ba0
// Retail codegen:
//   mov ax,[esp+4]; mov edx,[esp+8]; mov [ecx+4],ax; push edx; add ecx,8; call 0xc1bbe0; ret 8

typedef short EWeaponClass;

// sub-object living at this+8; its member fn is the tail-called 0xc1bbe0
struct SubObj {
    EWeaponClass* call(EWeaponClass* p);   // __fastcall member, one pushed arg
};

struct WVec {
    char pad0[4];              // +0
    EWeaponClass member4;      // +4
    char pad6[2];              // +6
    SubObj sub;                // +8
};

EWeaponClass* __fastcall vector_EWeaponClass_Umove(WVec* self, int /*edx*/,
                                                   EWeaponClass a, EWeaponClass* b)
{
    self->member4 = a;
    return self->sub.call(b);
}