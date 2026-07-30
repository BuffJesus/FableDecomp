#include <stdio.h>

// Stub for the external recalc routine tail-jumped to.
extern "C" void __fastcall CTCHeroStats_RecalcRenown() { }

// Function under test: exact naked transcription of retail 0x0057cae9.
__declspec(naked) void UpgradeRenownLevel()
{
    __asm {
        mov  eax, [ecx+0x74]
        inc  dword ptr [ecx+0x70]
        cmp  [ecx+0x70], eax
        jle  short skip
        mov  [ecx+0x70], eax
    skip:
        and  dword ptr [ecx+0x78], 0
        jmp  CTCHeroStats_RecalcRenown
    }
}

struct Obj {
    char pad[0x70];
    int  cur;    // +0x70
    int  cap;    // +0x74
    int  flag;   // +0x78
};

// Invoke with this=ecx (thiscall, no stack args).
static void call_upgrade(Obj* o)
{
    void (*fn)() = UpgradeRenownLevel;
    __asm {
        mov ecx, o
        call fn
    }
}

int main()
{
    // Case 1: cur below cap -> increments, flag cleared.
    Obj a; a.cur = 3; a.cap = 10; a.flag = 0x1234;
    call_upgrade(&a);
    if (a.cur != 4)   { printf("FAIL cur=%d\n", a.cur); return 1; }
    if (a.flag != 0)  { printf("FAIL flag=%d\n", a.flag); return 1; }

    // Case 2: cur already at cap -> increment then clamp back to cap.
    Obj b; b.cur = 10; b.cap = 10; b.flag = 7;
    call_upgrade(&b);
    if (b.cur != 10)  { printf("FAIL clamp cur=%d\n", b.cur); return 1; }
    if (b.flag != 0)  { printf("FAIL clamp flag=%d\n", b.flag); return 1; }

    // Case 3: cur == cap-1 -> becomes cap exactly (boundary, no clamp).
    Obj c; c.cur = 9; c.cap = 10; c.flag = 99;
    call_upgrade(&c);
    if (c.cur != 10)  { printf("FAIL boundary cur=%d\n", c.cur); return 1; }

    printf("OK_0x0057cae9\n");
    return 0;
}