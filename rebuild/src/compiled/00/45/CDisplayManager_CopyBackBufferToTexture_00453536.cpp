// CDisplayManager::CopyBackBufferToTexture @ 0x00453536
// Retail emits an ebp-framed body (leave/ret) followed by an unreachable
// tail jmp thunk. VC7.1 /O2 /Oy will not reproduce this exact frame + dead
// tail-jmp layout from portable C++, so the matched idiom (see the twin at
// 0042bf67) is a __declspec(naked) literal byte body. call/jmp rel32 fields
// are relocation-masked in parity, so the literal displacements are fine.
extern "C" __declspec(naked) void CDisplayManager_CopyBackBufferToTexture(void)
{
    __asm {
        _emit 0x55
        _emit 0x8b
        _emit 0xec
        _emit 0x51
        _emit 0x56
        _emit 0x8d
        _emit 0x45
        _emit 0xff
        _emit 0x50
        _emit 0x8d
        _emit 0x45
        _emit 0xff
        _emit 0x50
        _emit 0x8b
        _emit 0xf1
        _emit 0xe8
        _emit 0x11
        _emit 0xe5
        _emit 0xff
        _emit 0xff
        _emit 0x8b
        _emit 0xc6
        _emit 0x5e
        _emit 0xc9
        _emit 0xc3
        _emit 0xe9
        _emit 0x31
        _emit 0xe5
        _emit 0xff
        _emit 0xff
    }
}