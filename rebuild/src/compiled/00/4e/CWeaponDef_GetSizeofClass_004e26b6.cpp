// CWeaponDef::GetSizeofClass @ 0x004e26b6
// Retail: mov eax, 0xe4 ; ret  (returns sizeof-of-class constant)
struct CWeaponDef;

unsigned long __fastcall GetSizeofClass(CWeaponDef *self)
{
    (void)self;
    return 0xe4;
}