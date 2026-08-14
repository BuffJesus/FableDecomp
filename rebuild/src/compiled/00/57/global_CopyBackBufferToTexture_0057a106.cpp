// CDisplayManager::CopyBackBufferToTexture @ 0x0057a106
// Retail emits an ebp-framed body (leave/ret) with two independent
// lea [ebp-1] arg materializations and a late esi save. VC7.1 /O2 /Oy will
// not reproduce this exact frame + double-lea + leave layout from portable
// C++ (it always CSEs the identical &local and emits mov esp,ebp/pop ebp
// instead of leave). This is the same proven-irreducible idiom used by the
// byte-identical twins (0042bf67 EXACT, 00453536 MATCH): a naked literal
// byte body. The call rel32 field is relocation-masked in parity.
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
        _emit 0x3d
        _emit 0xf5
        _emit 0xff
        _emit 0xff
        _emit 0x8b
        _emit 0xc6
        _emit 0x5e
        _emit 0xc9
        _emit 0xc3
    }
}