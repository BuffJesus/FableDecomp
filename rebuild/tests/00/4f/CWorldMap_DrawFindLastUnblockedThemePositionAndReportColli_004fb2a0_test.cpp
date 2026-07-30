#include <stdio.h>

// Captured `this` (ecx) as seen by the real target after the thunk adjusts it.
static unsigned int g_seen_ecx = 0;

// Stub for the real method body the thunk tail-jumps to.
// It records ecx (the adjusted this pointer) then returns.
extern "C" __declspec(naked) void DrawFindLastUnblockedThemePositionAndReportCollision_target(void)
{
    __asm
    {
        mov     g_seen_ecx, ecx
        ret
    }
}

// The function-under-test (adjustor thunk).
extern "C" __declspec(naked) void DrawFindLastUnblockedThemePositionAndReportCollision(void)
{
    __asm
    {
        add     ecx, -4
        jmp     DrawFindLastUnblockedThemePositionAndReportCollision_target
    }
}

int main()
{
    unsigned int base = 0x00100000u;
    // Call the thunk with ecx = base; expect target to see base - 4.
    __asm
    {
        mov     ecx, base
        call    DrawFindLastUnblockedThemePositionAndReportCollision
    }

    if (g_seen_ecx != base - 4u)
    {
        printf("FAIL seen=0x%08X expected=0x%08X\n", g_seen_ecx, base - 4u);
        return 1;
    }

    // Second value to confirm it is a real subtraction, not a fixed constant.
    unsigned int base2 = 0x00200008u;
    __asm
    {
        mov     ecx, base2
        call    DrawFindLastUnblockedThemePositionAndReportCollision
    }
    if (g_seen_ecx != base2 - 4u)
    {
        printf("FAIL2 seen=0x%08X expected=0x%08X\n", g_seen_ecx, base2 - 4u);
        return 1;
    }

    printf("OK_0x004fb2a0 this_adjust=-4 verified\n");
    return 0;
}