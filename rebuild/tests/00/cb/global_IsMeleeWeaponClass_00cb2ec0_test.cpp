#include <stdio.h>

unsigned char g_flagByte = 0;
unsigned short g_flagWord = 0;

int IsMeleeWeaponClass(int weaponClass);

int main()
{
    int fails = 0;

    // both zero -> true
    g_flagByte = 0; g_flagWord = 0;
    if (IsMeleeWeaponClass(3) != 1) fails++;

    // byte set, word zero -> true
    g_flagByte = 1; g_flagWord = 0;
    if (IsMeleeWeaponClass(3) != 1) fails++;

    // byte zero, word set -> true
    g_flagByte = 0; g_flagWord = 5;
    if (IsMeleeWeaponClass(3) != 1) fails++;

    // both set -> false
    g_flagByte = 1; g_flagWord = 5;
    if (IsMeleeWeaponClass(3) != 0) fails++;

    if (fails == 0) printf("MELEEWEAPONCLASS_OK\n");
    else printf("FAIL %d\n", fails);
    return fails == 0 ? 0 : 1;
}