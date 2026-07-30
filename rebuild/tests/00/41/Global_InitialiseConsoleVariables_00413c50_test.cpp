#include <setjmp.h>
#include <stdio.h>
#include "fable_gfmain.h"

namespace
{
    jmp_buf g_jump;
    int g_scenario;
    int g_reason;
    fable_u32 g_observedAllocationSize;
    void* g_observedResetThis;
    void* g_observedConstructorThis;
    void* g_observedInitialiseThis;
    fable_u8 g_allocation[0x9C];

    void Abort(int reason)
    {
        g_reason = reason;
        longjmp(g_jump, 1);
    }
}

extern "C" fable_u32 g_FableConsoleRef_00413800 = 0;
extern "C" fable_u32 g_FableConsoleRef_00413840 = 0;
extern "C" fable_u32 g_FableConsoleRef_00413880 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E5B0 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E5C8 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E638 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E65C = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E674 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E68C = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E6B4 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E6C4 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E6D8 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E6EC = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E700 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E718 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E72C = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E73C = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E75C = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E77C = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E790 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E7A4 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E7B8 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E7CC = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E7E4 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E7FC = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E80C = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E824 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E840 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E850 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E85C = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E874 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E884 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E8A0 = 0;
extern "C" fable_u32 g_FableConsoleRef_0122E8B4 = 0;
extern "C" fable_u32 g_FableConsoleRef_01375444 = 0;
extern "C" fable_u32 g_FableConsoleRef_01375459 = 0;
extern "C" fable_u32 g_FableConsoleRef_01375494 = 0;
extern "C" fable_u32 g_FableConsoleRef_01375498 = 0;
extern "C" fable_u32 g_FableConsoleRef_0137549C = 0;
extern "C" fable_u32 g_FableConsoleRef_013754A0 = 0;
extern "C" fable_u32 g_FableConsoleRef_013754A4 = 0;
extern "C" fable_u32 g_FableConsoleRef_013754A8 = 0;
extern "C" fable_u32 g_FableConsoleRef_013754AC = 0;
extern "C" fable_u32 g_FableConsoleRef_013754B0 = 0;
extern "C" fable_u32 g_FableConsoleRef_013754B4 = 0;
extern "C" fable_u32 g_FableConsoleRef_013754B8 = 0;
extern "C" fable_u32 g_FableConsoleRef_013754BC = 0;
extern "C" fable_u32 g_FableConsoleRef_013754C0 = 0;
extern "C" fable_u32 g_FableConsoleRef_013B85F2 = 0;
extern "C" fable_u32 g_FableConsoleRef_013B8609 = 0;
extern "C" fable_u32 g_FableConsoleRef_013B860A = 0;
extern "C" fable_u32 g_FableConsoleRef_013B8615 = 0;
extern "C" fable_u32 g_FableConsoleRef_013B8616 = 0;
extern "C" fable_u32 g_FableConsoleRef_013B8617 = 0;
extern "C" fable_u32 g_FableConsoleRef_013B8618 = 0;
extern "C" fable_u32 g_FableConsoleRef_013B8619 = 0;
extern "C" fable_u32 g_FableConsoleRef_013B8646 = 0;
extern "C" fable_u32 g_FableConsoleRef_013CAA40 = 0;

extern "C" void* FableConsoleCall_00BFEA1A(fable_u32 size)
{
    g_observedAllocationSize = size;
    if (g_scenario == 2)
        return g_allocation;
    if (g_scenario == 1)
        return 0;
    Abort(90);
    return 0;
}

extern "C" void FableConsoleCall_009ECD80()
{
    __asm mov g_observedConstructorThis, ecx
    Abort(2);
}

extern "C" void FableConsoleCall_00413520()
{
    __asm mov g_observedResetThis, ecx
    Abort(g_scenario);
}

extern "C" void FableConsoleCall_009ED190()
{
    __asm mov g_observedInitialiseThis, ecx
    Abort(3);
}

extern "C" void FableConsoleCall_00414C90()
{
    Abort(99);
}

extern "C" void FableConsoleCall_00999230()
{
    Abort(99);
}

extern "C" void FableConsoleCall_0099B510()
{
    Abort(99);
}

extern "C" void FableConsoleCall_0099B6B0()
{
    Abort(99);
}

extern "C" void FableConsoleCall_0099EAE0()
{
    Abort(99);
}

extern "C" void FableConsoleCall_0099EBF0()
{
    Abort(99);
}

extern "C" void FableConsoleCall_0099EC30()
{
    Abort(99);
}

extern "C" void FableConsoleCall_009EBB30()
{
    Abort(99);
}

extern "C" void FableConsoleCall_009EC5E0()
{
    Abort(99);
}

extern "C" void FableConsoleCall_009EC890()
{
    Abort(99);
}

static int RunScenario(int scenario, fable_u32 singleton)
{
    g_scenario = scenario;
    g_reason = 0;
    g_observedAllocationSize = 0;
    g_observedResetThis = 0;
    g_observedConstructorThis = 0;
    g_observedInitialiseThis = 0;
    g_FableConsoleRef_013CAA40 = singleton;
    if (setjmp(g_jump) == 0)
    {
        InitialiseConsoleVariables();
        return 100;
    }
    return 0;
}

int main()
{
    if (RunScenario(1, 0) != 0 ||
        g_reason != 1 ||
        g_observedAllocationSize != 0x9C ||
        g_observedResetThis != &g_FableConsoleRef_013CAA40)
        return 1;

    if (RunScenario(2, 0) != 0 ||
        g_reason != 2 ||
        g_observedAllocationSize != 0x9C ||
        g_observedConstructorThis != g_allocation)
        return 2;

    if (RunScenario(3, 0x12345678UL) != 0 ||
        g_reason != 3 ||
        g_observedAllocationSize != 0 ||
        g_observedInitialiseThis != reinterpret_cast<void*>(0x12345678UL))
        return 3;

    puts("FABLETLC_INITIALISE_CONSOLE_VARIABLES PASS");
    return 0;
}
