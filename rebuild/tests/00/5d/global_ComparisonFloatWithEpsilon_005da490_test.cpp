#include <cstdio>

int g_divisor = 100;

struct Obj {
    char pad[0x14];
    int count;
    char flag;
};

static char __fastcall Comparison_Float_WithEpsilon(Obj* self, void* edx, float arg)
{
    if (self->flag) return 1;
    return (float)self->count / g_divisor > arg;
}

static bool call_it(Obj* self, float arg)
{
    unsigned char r;
    __asm {
        mov ecx, self
        fld arg
        sub esp, 4
        fstp dword ptr [esp]
        call Comparison_Float_WithEpsilon
        mov r, al
    }
    return r != 0;
}

int main()
{
    Obj o;
    o.count = 500;
    o.flag = 0;
    // 500/100 = 5.0; 5.0 > 4.0 -> true
    if (call_it(&o, 4.0f) != true) { printf("FAIL1\n"); return 1; }
    // 5.0 > 6.0 -> false
    if (call_it(&o, 6.0f) != false) { printf("FAIL2\n"); return 1; }
    // 5.0 > 5.0 -> false (strict)
    if (call_it(&o, 5.0f) != false) { printf("FAIL3\n"); return 1; }
    // flag set -> true regardless
    o.flag = 1;
    if (call_it(&o, 1000.0f) != true) { printf("FAIL4\n"); return 1; }
    printf("OK_0x005da490\n");
    return 0;
}