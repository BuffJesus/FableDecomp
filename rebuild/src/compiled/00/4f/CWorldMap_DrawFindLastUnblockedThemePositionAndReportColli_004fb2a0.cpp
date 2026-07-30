// 0x004fb2a0 CWorldMap::DrawFindLastUnblockedThemePositionAndReportCollision
// MSVC multiple-inheritance adjustor thunk:
//     83 c1 fc          add ecx, -4         ; this -= 4
//     e9 88 28 14 00    jmp 0x0063db30      ; tail-call real method body
// The jmp target is an external symbol (reloc-masked in the parity check).

extern "C" void DrawFindLastUnblockedThemePositionAndReportCollision_target(void);

extern "C" __declspec(naked) void DrawFindLastUnblockedThemePositionAndReportCollision(void)
{
    __asm
    {
        add     ecx, -4
        jmp     DrawFindLastUnblockedThemePositionAndReportCollision_target
    }
}