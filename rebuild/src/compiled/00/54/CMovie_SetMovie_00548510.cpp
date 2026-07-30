// CMovie::SetMovie @ 0x00548510
// Adjustor thunk: advances the this pointer by +0x174 (to a base-class
// subobject), then tail-calls the real implementation at 0x0099b7d0.
//   81 c1 74 01 00 00   add ecx, 0x174
//   e9 b5 32 45 00      jmp 0x0099b7d0
// The jmp displacement is a relocation, so the extern target is reloc-masked.
extern "C" void RealTarget_0x99b7d0(void);

__declspec(naked) void SetMovie(void)
{
    __asm
    {
        add ecx, 0x174
        jmp RealTarget_0x99b7d0
    }
}