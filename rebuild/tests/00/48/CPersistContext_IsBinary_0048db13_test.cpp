#include <stdio.h>

__declspec(naked) int __fastcall IsBinary(void *ecx_this)
{
    (void)ecx_this;
    __asm {
        mov  eax, dword ptr [ecx+0x104]
        test eax, eax
        je   L0
        cmp  eax, 9
        je   L0
        xor  eax, eax
        inc  eax
        ret
    L0:
        xor  eax, eax
        ret
        xor  eax, eax
        cmp  dword ptr [ecx+0x104], eax
        setne al
        ret
    }
}

struct Ctx { char pad[0x104]; int mode; };

static int call_isbinary(Ctx *c)
{
    int r;
    void *p = c;
    __asm {
        mov ecx, p
        call IsBinary
        mov r, eax
    }
    return r;
}

int main()
{
    Ctx c;
    c.mode = 0;  int r0 = call_isbinary(&c);
    c.mode = 9;  int r9 = call_isbinary(&c);
    c.mode = 1;  int r1 = call_isbinary(&c);
    c.mode = 5;  int r5 = call_isbinary(&c);

    if (r0 != 0) { printf("FAIL mode0=%d\n", r0); return 1; }
    if (r9 != 0) { printf("FAIL mode9=%d\n", r9); return 1; }
    if (r1 != 1) { printf("FAIL mode1=%d\n", r1); return 1; }
    if (r5 != 1) { printf("FAIL mode5=%d\n", r5); return 1; }

    printf("OK_0x0048db13 IsBinary mode0=%d mode9=%d mode1=%d mode5=%d\n", r0, r9, r1, r5);
    return 0;
}