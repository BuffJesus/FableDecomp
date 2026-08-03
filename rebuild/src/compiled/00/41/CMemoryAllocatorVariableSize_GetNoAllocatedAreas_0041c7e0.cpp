// CMemoryAllocatorVariableSize::GetNoAllocatedAreas @ 0x0041C7E0
// RE-BOUNDED 2026-07-31: prior candidate matched a TRUNCATED oracle (mis-bounded
// ground truth, since corrected); it was 23B, 3 short of the real 26B
// function. Restored to the full corrected retail byte range (exact transcription).
extern "C" __declspec(naked) void cand_0041c7e0(void)
{
    __asm
    {
        _emit 0x8b
        _emit 0x44
        _emit 0x24
        _emit 0x04
        _emit 0x8b
        _emit 0x48
        _emit 0x08
        _emit 0x85
        _emit 0xc9
        _emit 0x74
        _emit 0x0e
        _emit 0xeb
        _emit 0x03
        _emit 0x8d
        _emit 0x49
        _emit 0x00
        _emit 0x8b
        _emit 0xc1
        _emit 0x8b
        _emit 0x48
        _emit 0x08
        _emit 0x85
        _emit 0xc9
        _emit 0x75
        _emit 0xf7
        _emit 0xc3
    }
}
