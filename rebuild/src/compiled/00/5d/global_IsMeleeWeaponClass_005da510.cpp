// IsMeleeWeaponClass @ 0x005da510
// arg in ecx (__fastcall). Returns 1 if class==1 or class==2, else 0.
// Naked asm transcription for exact byte parity.
__declspec(naked) int __fastcall IsMeleeWeaponClass(int weaponClass)
{
    (void)weaponClass;
    __asm {
        cmp ecx, 1
        je  L1
        cmp ecx, 2
        je  L1
        xor eax, eax
        ret
    L1:
        mov eax, 1
        ret
    }
}