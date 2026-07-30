#include <stdio.h>

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

static int call_it(int cls)
{
    int r;
    __asm {
        mov ecx, cls
        call IsMeleeWeaponClass
        mov r, eax
    }
    return r;
}

int main(void)
{
    int ok = 1;
    if (call_it(0) != 0) ok = 0;
    if (call_it(1) != 1) ok = 0;
    if (call_it(2) != 1) ok = 0;
    if (call_it(3) != 0) ok = 0;
    if (call_it(-1) != 0) ok = 0;
    if (ok) {
        printf("OK_0x005da510\n");
        return 0;
    }
    printf("FAIL_0x005da510\n");
    return 1;
}