#include <stdio.h>
struct CTCInventoryExperience { char pad[0x1bc]; int m_value; int m_max; };

__declspec(naked) void __fastcall IncrementStatIndicator(CTCInventoryExperience *thisp)
{
    (void)thisp;
    __asm {
        lea eax, [ecx+0x1bc]
        mov edx, [eax]
        inc edx
        cmp edx, [ecx+0x1c0]
        jge skip
        mov [eax], edx
    skip:
        ret
    }
}

static void call_it(CTCInventoryExperience *o)
{
    void *fn = (void*)&IncrementStatIndicator;
    __asm {
        mov ecx, o
        call fn
    }
}

int main()
{
    CTCInventoryExperience o;
    o.m_value = 5; o.m_max = 10;
    call_it(&o);
    if (o.m_value != 6) { printf("FAIL inc: %d\n", o.m_value); return 1; }
    o.m_value = 9; call_it(&o);
    if (o.m_value != 9) { printf("FAIL cap-boundary: %d\n", o.m_value); return 1; }
    o.m_value = 8; call_it(&o);
    if (o.m_value != 9) { printf("FAIL near: %d\n", o.m_value); return 1; }
    o.m_value = 10; call_it(&o);
    if (o.m_value != 10) { printf("FAIL atmax: %d\n", o.m_value); return 1; }
    printf("OK_0x0056e5ca value=%d\n", o.m_value);
    return 0;
}