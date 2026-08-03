// Thunk at 0x004fb290: adjusts the this pointer by -4, then tail-calls the
// real method at 0x0063d960.
//   83 c1 fc          add ecx, -4
//   e9 c8 26 14 00    jmp 0x0063d960
extern "C" void RealTarget_0x63d960(void);

__declspec(naked) void DrawFindLastUnblockedGroundPositionAndReportCollision(void)
{
    __asm
    {
        add ecx, -4
        jmp RealTarget_0x63d960
    }
}