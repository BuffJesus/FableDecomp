// IsMeleeWeaponClass @ 00cb2ec0
// bool __fastcall IsMeleeWeaponClass(EWeaponClass)
// param unused; reads two globals.
//   if (g_flagByte && g_flagWord) return false; else return true;

extern unsigned char g_flagByte;   // byte ptr [0x143e8ae]
extern unsigned short g_flagWord;  // word ptr [0x143cf10]

int IsMeleeWeaponClass(int weaponClass)
{
    if (g_flagByte)
    {
        if (g_flagWord)
            return 0;
    }
    return 1;
}